/******************************************************************************
** WordTrain 0.9.1 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov <kharvd@gmail.com>
**
** This file is part of WordTrain.
**
** $QT_BEGIN_LICENSE:GPL$
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QT_END_LICENSE$
**
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#include "mainwindow.h"

#include <QEvent>
#include <QTableWidget>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QToolButton>
#include "xmlreader.h"
#include "xmlwriter.h"
#include "viewcarddialog.h"
#include "startquizdialog.h"
#include "quizdialog.h"
#include "neweditcarddialog.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "gettingstartedwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolbars();
    createSearchBar();
    createStatusBar();
    createStartingWidget();

    // "---" means 'empty file'
    setCurrentFile("---");
    setUnifiedTitleAndToolBarOnMac(true);

    readSettings();

    editActionsState();

    QSettings settings;

    if (settings.value("save_pos").toString().isEmpty())
        settings.setValue("save_pos", true);

    if (settings.value("save_pos").toBool()) {
        QPoint pos = settings.value("pos", QPoint(defaultXPosition,
                                                  defaultYPosition)).toPoint();
        QSize size = settings.value("size", QSize(defaultWidth,
                                                  defaultHeight)).toSize();
        restoreState(settings.value("window_state").toByteArray());
        bool isMax = settings.value("maximized", false).toBool();
        resize(size);
        move(pos);
        isMax ? showMaximized() : showNormal();
    } else {
        resize(defaultWidth, defaultHeight);
        move(defaultXPosition, defaultYPosition);
    }

    mSearching = false;
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == tableWords) {
        if (event->type() == QEvent::ContextMenu) {
            QContextMenuEvent *contextMenuEvent
                    = static_cast<QContextMenuEvent *>(event);

            QMenu menu(this);
            menu.addAction(actionAddCard);
            menu.addAction(actionViewCard);
            menu.addAction(actionEditCard);
            menu.addAction(actionDeleteCard);
            menu.exec(contextMenuEvent->globalPos());

            return true;
        } else {
            return false;
        }
    } else {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}

void MainWindow::newSet()
{
    if (!isFileOpened())
        createTableWidget();

    if (maybeSave()) {
        setCurrentFile("");
        setWindowModified(true);
        mCards.clear();
        tableWords->show();
        updateTable(mCards);
        editActionsState();
    }
}

void MainWindow::openSet()
{
    if (maybeSave()) {
        QString fileName =
                QFileDialog::getOpenFileName(this, tr("Open set"), "",
                                             tr("Words set file (*.wsf);;"
                                                "XML files (*.xml);;"
                                                "All files (*)"));

        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::saveSet()
{
    if (mCurrentFile.isEmpty())
        return saveSetAs();
    else
        return saveFile(mCurrentFile);
}

bool MainWindow::saveSetAs()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save set"), "",
                                         tr("Words set file (*.wsf);;"
                                            "XML files (*.xml);;"
                                            "All files (*)"));
    if (!fileName.isEmpty())
        return saveFile(fileName);

    return false;
}

void MainWindow::addCard()
{
    NewEditCardDialog *dlg = new NewEditCardDialog(this);
    if (dlg->exec()) {
        // Getting new card's contents
        mCards.push_back(dlg->getNewCard());

        // Set is modified now
        setWindowModified(true);
        updateTable(mCards);

        // Setting selection to the new card
        tableWords->setCurrentCell(tableWords->rowCount() - 1, 0);
    }

    delete dlg;
}

void MainWindow::editCard()
{
    int curr = tableWords->currentRow();
    if (isInRange(curr)) {
        NewEditCardDialog* dlg;

        if (mSearching)
            dlg = new NewEditCardDialog(*mSearchResults.at(curr), this);
        else
            dlg = new NewEditCardDialog(mCards.at(curr), this);

        if (dlg->exec()) {
            // Updating card's contents
            if (mSearching) {
                *mSearchResults[curr] = dlg->getNewCard();
                updateTable(mSearchResults);
            } else {
                mCards.replace(curr, dlg->getNewCard());
                updateTable(mCards);
            }

            // Set is modified now
            setWindowModified(true);

            // Selecting edited card
            tableWords->setCurrentCell(curr, 0);
        }

        delete dlg;
    }
}

void MainWindow::viewCard() {
    showCard(tableWords->currentRow());
}

void MainWindow::deleteCard()
{
    int curr = tableWords->currentRow();
    if (isInRange(curr)) {
        // Removing card
        if (mSearching)
            mCards.removeOne(*mSearchResults.at(curr));
        else
            mCards.removeAt(curr);

        mSearching = false;
        editSearch->clear();

        // Set is modified now
        setWindowModified(true);

        updateTable(mCards);

        // Selecting previous or the first card
        if (curr != 0)
            tableWords->setCurrentCell(curr - 1, 0);
        else
            tableWords->setCurrentCell(0, 0);
    }
}

void MainWindow::importSet()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Import set"), "",
                                         tr("Words set file (*.wsf);;"
                                            "XML files (*.xml);;"
                                            "All files (*)"));
    if (!fileName.isEmpty()) {
        loadFile(fileName, true);
        setWindowModified(true);
    }
}

void MainWindow::settings()
{
    SettingsDialog *dlgSettings = new SettingsDialog(this);

    if (dlgSettings->exec()) {
        readSettings();
        if (isFileOpened())
            updateTable(mCards);
    }

    delete dlgSettings;
}

void MainWindow::startTraining()
{
    showCard(0);
}

void MainWindow::startQuiz()
{
    // Set shouldn't be empty
    if (tableWords->rowCount()) {
        StartQuizDialog* dlg = new StartQuizDialog(&mCards, this);

        if (dlg->exec()) {
            if (dlg->getCards().size()) {
                QuizDialog *quizDlg = new QuizDialog(dlg->getCards(),
                                                     dlg->getChoiceMode(),
                                                     dlg->getHideMode(),
                                                     getPointersSet(), this);
                tableWords->hide();
                quizDlg->exec();
                tableWords->show();

                if (quizDlg->isModified()) {
                    setWindowModified(true);
                    updateTable(mCards);
                }

                delete quizDlg;
            } else {
                // If nothing has been selected
                QMessageBox::critical(this, tr("Error"),
                           tr("No words have been selected!"),
                           QMessageBox::Ok,
                           QMessageBox::Ok);
            }
        }
        delete dlg;
    }
}

void MainWindow::about()
{
    AboutDialog *dlg = new AboutDialog(this);
    dlg->exec();
    delete dlg;
}

void MainWindow::search(QString str)
{
    mSearchResults.clear();

    if (!str.isEmpty()) {
        mSearching = true;

        for (int i = 0; i < mCards.size(); i++) {
            if (mCards.at(i).word().contains(str, Qt::CaseInsensitive)
             || mCards.at(i).translation().contains(str, Qt::CaseInsensitive)) {
                mSearchResults.push_back(&mCards[i]);
            }
        }
        updateTable(mSearchResults);
    } else {
        mSearching = false;
        updateTable(mCards);
    }
}

void MainWindow::createStartingWidget()
{
    wgtGettingStarted = new GettingStartedWidget;
    connect(wgtGettingStarted, SIGNAL(newSet()), SLOT(newSet()));
    connect(wgtGettingStarted, SIGNAL(openSet()), SLOT(openSet()));
    connect(wgtGettingStarted, SIGNAL(help()), SLOT(about()));
    connect(wgtGettingStarted, SIGNAL(quit()), SLOT(close()));

    setCentralWidget(wgtGettingStarted);
}

void MainWindow::createTableWidget()
{
    tableWords = new QTableWidget();

    tableWords->setColumnCount(4);
    tableWords->setRowCount(0);

    QStringList headerLabels;
    headerLabels << tr("Word") << tr("Transcription") << tr("Translation")
            << tr("Progress");
    tableWords->setHorizontalHeaderLabels(headerLabels);

    // Select whole row without multi-selection.
    tableWords->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWords->setSelectionMode(QAbstractItemView::SingleSelection);

    tableWords->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    tableWords->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    tableWords->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    tableWords->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);

    // No edit triggers
    tableWords->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Installing event filter for handling context menu event
    tableWords->installEventFilter(this);
    connect(tableWords, SIGNAL(doubleClicked(QModelIndex)), SLOT(viewCard()));

    tableWords->hide();
    setCentralWidget(tableWords);
}

void MainWindow::createActions()
{
    // New set
    actionNewSet = new QAction(QIcon::fromTheme("document-new",
                               QIcon(":/icons/new.png")),
                               tr("&New set..."), this);
    actionNewSet->setShortcuts(QKeySequence::New);
    actionNewSet->setStatusTip(tr("Create a new card set"));
    connect(actionNewSet, SIGNAL(triggered()), this, SLOT(newSet()));

    // Open set
    actionOpenSet = new QAction(QIcon::fromTheme("document-open",
                                QIcon(":/icons/open.png")),
                                tr("&Open set..."), this);
    actionOpenSet->setShortcuts(QKeySequence::Open);
    actionOpenSet->setStatusTip(tr("Open an existing card set"));
    connect(actionOpenSet, SIGNAL(triggered()), this, SLOT(openSet()));

    // Save set
    actionSaveSet = new QAction(QIcon::fromTheme("document-save",
                                QIcon(":/icons/save.png")),
                                tr("&Save set"), this);
    actionSaveSet->setShortcuts(QKeySequence::Save);
    actionSaveSet->setStatusTip(tr("Save the card set to disk"));
    connect(actionSaveSet, SIGNAL(triggered()), this, SLOT(saveSet()));

    // Save set as
    actionSaveSetAs = new QAction(tr("Save set &As..."), this);
    actionSaveSetAs->setShortcuts(QKeySequence::SaveAs);
    actionSaveSetAs->setStatusTip(tr("Save the card set under a new name"));
    connect(actionSaveSetAs, SIGNAL(triggered()), this, SLOT(saveSetAs()));

    // Quit
    actionQuit = new QAction(QIcon::fromTheme("application-exit",
                             QIcon(":/icons/quit.png")),
                             tr("&Quit"), this);
    actionQuit->setShortcuts(QKeySequence::Quit);
    actionQuit->setStatusTip(tr("Quit the application"));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    // Add card
    actionAddCard = new QAction(QIcon::fromTheme("list-add",
                                QIcon(":/icons/add.png")),
                                tr("&Add card..."), this);
    actionAddCard->setShortcut(QKeySequence("Ctrl+A"));
    actionAddCard->setStatusTip(tr("Add a new word card to the set"));
    connect(actionAddCard, SIGNAL(triggered()), this, SLOT(addCard()));

    // View card
    actionViewCard = new QAction(QIcon::fromTheme("edit-select-all",
                                 QIcon(":/icons/view.png")),
                                 tr("&View card..."), this);
    actionViewCard->setStatusTip(tr("View current word card"));
    connect(actionViewCard, SIGNAL(triggered()), this, SLOT(viewCard()));

    // Edit card
    actionEditCard = new QAction(QIcon(":/icons/edit.png"),
                                 tr("&Edit card..."), this);
    actionEditCard->setStatusTip(tr("Edit current word card"));
    connect(actionEditCard, SIGNAL(triggered()), this, SLOT(editCard()));

    // Delete card
    actionDeleteCard = new QAction(QIcon::fromTheme("list-remove",
                                   QIcon(":/icons/remove.png")),
                                   tr("&Delete card"), this);
    actionDeleteCard->setShortcuts(QKeySequence::Delete);
    actionDeleteCard->setStatusTip(tr("Delete the word card from the set"));
    connect(actionDeleteCard, SIGNAL(triggered()), this, SLOT(deleteCard()));

    // Import set
    actionImportSet = new QAction(tr("&Import set..."), this);
    actionImportSet->setStatusTip(
        tr("Append an existing set to the current"));
    connect(actionImportSet, SIGNAL(triggered()), this, SLOT(importSet()));

    // Settings
    actionSettings = new QAction(QIcon::fromTheme("document-properties",
                                 QIcon(":/icons/settings.png")),
                                 tr("Settings..."), this);
    actionSettings->setStatusTip(tr("Settings"));
    connect(actionSettings, SIGNAL(triggered()),
            this, SLOT(settings()));

    // Start training
    actionStartTraining = new QAction(tr("Start &training..."), this);
    actionStartTraining->setStatusTip(tr("Start learning words from the set"));
    connect(actionStartTraining, SIGNAL(triggered()),
            this, SLOT(startTraining()));

    // Start quiz
    actionStartQuiz = new QAction(QIcon::fromTheme("media-playback-start",
                                  QIcon(":/icons/start_quiz.png")),
                                  tr("Start qui&z..."), this);
    actionStartQuiz->setShortcut(QKeySequence("Ctrl+T"));
    actionStartQuiz->setStatusTip(
            tr("Test your knowledge of words from the set"));
    connect(actionStartQuiz, SIGNAL(triggered()),
            this, SLOT(startQuiz()));

    // About
    actionAbout = new QAction(tr("&About"), this);
    actionAbout->setStatusTip(tr("Show the application's About box"));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    // About Qt
    actionAboutQt = new QAction(tr("About &Qt"), this);
    actionAboutQt->setStatusTip(tr("Show the Qt library's About box"));
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    // File
    menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionNewSet);
    menuFile->addAction(actionOpenSet);
    menuFile->addAction(actionSaveSet);
    menuFile->addAction(actionSaveSetAs);

    menuFile->addSeparator();

    menuFile->addAction(actionQuit);

    // Edit
    menuEdit = menuBar()->addMenu(tr("&Edit"));
    menuEdit->addAction(actionAddCard);
    menuEdit->addAction(actionViewCard);
    menuEdit->addAction(actionEditCard);
    menuEdit->addAction(actionDeleteCard);
    menuEdit->addAction(actionImportSet);
    menuEdit->addAction(actionSettings);

    // Training
    menuTraining = menuBar()->addMenu(tr("&Training"));
    menuTraining->addAction(actionStartTraining);
    menuTraining->addAction(actionStartQuiz);

    menuBar()->addSeparator();

    // About
    menuAbout = menuBar()->addMenu(tr("&About"));
    menuAbout->addAction(actionAbout);
    menuAbout->addAction(actionAboutQt);
}

void MainWindow::createToolbars()
{
    toolBar = addToolBar(tr("Toolbar"));
    toolBar->addAction(actionNewSet);
    toolBar->addAction(actionOpenSet);
    toolBar->addAction(actionSaveSet);

    toolBar->addSeparator();

    toolBar->addAction(actionAddCard);
    toolBar->addAction(actionDeleteCard);
    toolBar->addAction(actionEditCard);
    toolBar->addAction(actionViewCard);
    toolBar->addAction(actionStartQuiz);

    toolBar->addSeparator();

    toolBar->addAction(actionSettings);
    toolBar->addAction(actionQuit);

    toolBar->addSeparator();
}

void MainWindow::createSearchBar()
{
    editSearch = new QLineEdit();
    connect(editSearch, SIGNAL(textChanged(QString)), SLOT(search(QString)));

    QToolButton *btnClear = new QToolButton();
    btnClear->setToolTip(tr("Clear"));
    btnClear->setIcon(QIcon(":/icons/fileclose.png"));
    btnClear->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    connect(btnClear, SIGNAL(clicked()), editSearch, SLOT(clear()));

    QHBoxLayout *lt = new QHBoxLayout;
    lt->addStretch(1);
    lt->addWidget(new QLabel(tr("Search")));
    lt->addWidget(editSearch);
    lt->addWidget(btnClear);

    QWidget *searchWgt = new QWidget();
    searchWgt->setLayout(lt);

    toolBar->addWidget(searchWgt);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings;
    corrAnsForLearned = settings.value("corr_answers", 10).toInt();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    if (settings.value("save_pos").toBool()) {
        settings.setValue("pos", pos());

        if (isMaximized())
            settings.setValue("size", QSize(defaultWidth, defaultHeight));
        else
            settings.setValue("size", size());

        settings.setValue("maximized", isMaximized());
        settings.setValue("window_state", saveState());
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    mCurrentFile = fileName;
    setWindowModified(false);

    QString shownName = mCurrentFile;

    if (isFileOpened()) {
        if (mCurrentFile.isEmpty())
            shownName = "untitled.wsf";

        setWindowTitle(shownName + "[*]" + " \u2014 "
                       + qApp->applicationName());
    } else {
        setWindowTitle(qApp->applicationName());
    }
}

void MainWindow::loadFile(const QString &fileName, bool import)
{
    WordsSet temp;
    XmlReader reader(&temp);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    bool noErrors = reader.readFile(fileName);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (!isFileOpened())
        createTableWidget();

    if (noErrors) {
        if (import) {
            mCards.append(temp);
            statusBar()->showMessage(tr("File imported"), 2000);
        } else {
            mCards = temp;
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("File loaded"), 2000);
        }

        tableWords->show();
        updateTable(mCards);
        editActionsState();
    } else {
        QMessageBox::critical(this, tr("Error"), reader.getErrorMessage(),
                              QMessageBox::Ok);
    }
}

bool MainWindow::saveFile(const QString & fileName)
{
    XmlWriter writer(&mCards);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    bool noError = writer.writeFile(fileName);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (noError) {
        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File saved"), 2000);

        return true;
    }

    QMessageBox::critical(this, tr("Error"), writer.getErrorMessage(),
                          QMessageBox::Ok);
    return false;
}

bool MainWindow::maybeSave()
{
    if (isWindowModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, qApp->applicationName(),
                                   tr("The set has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveSet();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::updateTable(WordsSet words)
{
    mSearching = false;
    editSearch->clear();

    tableWords->clearContents();
    tableWords->setRowCount(0);

    WordsSet::const_iterator it = words.constBegin();

    int rowCount;
    QTableWidgetItem* tmp;

    for ( ; it != words.constEnd(); it++) {
        // Old row count and index of last row
        rowCount = tableWords->rowCount();

        tableWords->insertRow(rowCount);

        // Word
        tmp = new QTableWidgetItem(it->word());
        tableWords->setItem(rowCount, 0, tmp);

        // Transcription
        tmp = new QTableWidgetItem(it->transcription());
        tableWords->setItem(rowCount, 1, tmp);

        // Translation
        tmp = new QTableWidgetItem(it->translation());
        tableWords->setItem(rowCount, 2, tmp);

        // Learning progress in %
        int progress = (static_cast<double>(it->correctAnswers())
                            / corrAnsForLearned * 100);
        progress = (progress > 100) ? 100 : progress;

        tmp = new QTableWidgetItem(QString("%1%").arg(progress));

        tmp->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWords->setItem(rowCount, 3, tmp);
    }

    tableWords->setCurrentCell(0, 0);
}

void MainWindow::updateTable(WordsPtrSet words) 
{
    tableWords->clearContents();
    tableWords->setRowCount(0);

    WordsPtrSet::const_iterator it = words.constBegin();

    int rowCount;
    QTableWidgetItem* tmp;

    for ( ; it != words.constEnd(); it++) {
        // Old row count and index of last row
        rowCount = tableWords->rowCount();

        tableWords->insertRow(rowCount);

        // Word
        tmp = new QTableWidgetItem((*it)->word());
        tableWords->setItem(rowCount, 0, tmp);

        // Transcription
        tmp = new QTableWidgetItem((*it)->transcription());
        tableWords->setItem(rowCount, 1, tmp);

        // Translation
        tmp = new QTableWidgetItem((*it)->translation());
        tableWords->setItem(rowCount, 2, tmp);

        // Learning progress in %
        int progress = (static_cast<double>((*it)->correctAnswers())
                            / corrAnsForLearned * 100);
        progress = (progress > 100) ? 100 : progress;

        tmp = new QTableWidgetItem(QString("%1%").arg(progress));

        tmp->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWords->setItem(rowCount, 3, tmp);
    }

    tableWords->setCurrentCell(0, 0);
}

bool MainWindow::isFileOpened()
{
    return (mCurrentFile != "---");
}

void MainWindow::editActionsState()
{
    bool state = isFileOpened();
    actionSaveSet->setEnabled(state);
    actionSaveSetAs->setEnabled(state);
    actionAddCard->setEnabled(state);
    actionViewCard->setEnabled(state);
    actionEditCard->setEnabled(state);
    actionDeleteCard->setEnabled(state);
    actionImportSet->setEnabled(state);
    actionStartTraining->setEnabled(state);
    actionStartQuiz->setEnabled(state);
    editSearch->setEnabled(state);
}

void MainWindow::showCard(int index)
{
    if (isInRange(index)) {
        ViewCardDialog *viewDlg;
        if (mSearching)
            viewDlg = new ViewCardDialog(mSearchResults, this);
        else
            viewDlg = new ViewCardDialog(&mCards, this);

        viewDlg->setCurrentCard(index);

        viewDlg->exec();

        if (viewDlg->isModified()) {
            setWindowModified(true);
            if (mSearching)
                updateTable(mSearchResults);
            else
                updateTable(mCards);
        }

        delete viewDlg;
    }
}

bool MainWindow::isInRange(int curr)
{
    return ((curr >= 0) && (curr < mCards.size()));
}

WordsPtrSet MainWindow::getPointersSet()
{
    WordsPtrSet tmp;
    for (int i = 0; i < mCards.size(); i++)
        tmp.push_back(&mCards[i]);

    return tmp;
}
