/******************************************************************************
** WordTrain 0.9.3 -- Foreign words trainer
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
#include <QComboBox>
#include "xmlreader.h"
#include "xmlwriter.h"
#include "viewcarddialog.h"
#include "startquizdialog.h"
#include "quizdialog.h"
#include "neweditcarddialog.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "gettingstartedwidget.h"
#include "wordfilter.h"
#include "tagwordfilter.h"
#include "searchwordfilter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolbars();
    createSearchTags();
    createStatusBar();
    createStartingWidget();

    // "---" means 'empty file'
    setCurrentFile("---");
    setUnifiedTitleAndToolBarOnMac(true);

    readSettings();

    editActionsState();

    m_Filtering = false;
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
            menuTableContextMenu->exec(contextMenuEvent->globalPos());

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
        m_Cards.clear();
        tableWords->show();
        updateTable(m_Cards);
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
    if (m_CurrentFile.isEmpty())
        return saveSetAs();
    else
        return saveFile(m_CurrentFile);
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
        m_Cards.push_back(dlg->newCard());

        if (m_Filtering) {
            search(txtSearch->text());
            updateTable(ptrsToWordsSet(m_FilteredSet));
        } else {
            updateTable(m_Cards);
        }

        // Set is modified now
        setWindowModified(true);

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

        if (m_Filtering)
            dlg = new NewEditCardDialog(*m_FilteredSet.at(curr), this);
        else
            dlg = new NewEditCardDialog(m_Cards.at(curr), this);

        if (dlg->exec()) {
            // Updating card's contents
            if (m_Filtering) {
                *m_FilteredSet[curr] = dlg->newCard();
                updateTable(ptrsToWordsSet(m_FilteredSet));
            } else {
                m_Cards.replace(curr, dlg->newCard());
                updateTable(m_Cards);
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
        if (m_Filtering) {
            m_Cards.removeOne(*m_FilteredSet.at(curr));
            m_FilteredSet.removeAt(curr);
            updateTable(ptrsToWordsSet(m_FilteredSet));
        } else {
            m_Cards.removeAt(curr);
            updateTable(m_Cards);
        }

        // Set is modified now
        setWindowModified(true);

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
            updateTable(m_Cards);
    }

    delete dlgSettings;
}

void MainWindow::startTraining()
{
    showCard(0);
}

void MainWindow::startQuiz()
{
    // Set mustn't be empty
    if (tableWords->rowCount()) {
        StartQuizDialog* dlg = new StartQuizDialog(getPointersSet(), this);

        if (dlg->exec()) {
            txtSearch->clear();
            updateTable(m_Cards);

            if (dlg->cards().size()) {
                QuizDialog *quizDlg = new QuizDialog(dlg->cards(),
                                                     dlg->choiceMode(),
                                                     dlg->hideMode(),
                                                     getPointersSet(), this);
                tableWords->hide();
                quizDlg->exec();
                tableWords->show();

                if (quizDlg->isModified()) {
                    setWindowModified(true);
                    updateTable(m_Cards);
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

void MainWindow::tagFilter(int index)
{
    if (index == 0 && txtSearch->text().isEmpty()) {
        m_Filtering = false;
        updateTable(m_Cards);
    } else {
        m_Filtering = true;

        TagWordFilter filter;
        WordsPtrSet tmp;

        if (txtSearch->text().isEmpty()) {
            tmp = getPointersSet();
        } else {
            m_FilteredSet.clear();
            search(txtSearch->text());
            tmp = m_FilteredSet;
        }

        if (index != 0)
            m_FilteredSet = filter.filter(cmbTags->itemData(index).toString(),
                                          tmp);

        updateTable(ptrsToWordsSet(m_FilteredSet));

    }
}

void MainWindow::search(const QString &str)
{
    if ((cmbTags->currentIndex() == 0) && txtSearch->text().isEmpty()) {
        m_Filtering = false;
        updateTable(m_Cards);
    } else {
        m_Filtering = true;

        int index = cmbTags->currentIndex();
        if (index != 0) {
            TagWordFilter filter;
            m_FilteredSet = filter.filter(cmbTags->itemData(index).toString(),
                                          getPointersSet());
        } else {
            m_FilteredSet = getPointersSet();
        }

        if (!txtSearch->text().isEmpty()) {
            SearchWordFilter filter;
            m_FilteredSet = filter.filter(str, m_FilteredSet);
        }

        updateTable(ptrsToWordsSet(m_FilteredSet));
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
    tableWords->setFocus();
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

    // Context menu
    menuTableContextMenu = new QMenu(this);
    menuTableContextMenu->addAction(actionAddCard);
    menuTableContextMenu->addAction(actionViewCard);
    menuTableContextMenu->addAction(actionEditCard);
    menuTableContextMenu->addAction(actionDeleteCard);
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

    toolBar->setObjectName("Toolbar");
}

void MainWindow::createSearchTags()
{
    txtSearch = new QLineEdit();
    connect(txtSearch, SIGNAL(textChanged(QString)), SLOT(search(QString)));

    QToolButton *btnClear = new QToolButton();
    btnClear->setToolTip(tr("Clear"));
    btnClear->setIcon(QIcon(":/icons/fileclose.png"));
    btnClear->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    connect(btnClear, SIGNAL(clicked()), txtSearch, SLOT(clear()));

    cmbTags = new QComboBox();
    connect(cmbTags, SIGNAL(currentIndexChanged(int)), SLOT(tagFilter(int)));

    QHBoxLayout *lt = new QHBoxLayout;
    lt->addWidget(new QLabel(tr("Filter:")));
    lt->addWidget(cmbTags, 1);
    lt->addStretch(2);
    lt->addWidget(new QLabel(tr("Search:")));
    lt->addWidget(txtSearch);
    lt->addWidget(btnClear);

    QWidget *tagsSearchWgt = new QWidget();
    tagsSearchWgt->setLayout(lt);

    toolBar->addWidget(tagsSearchWgt);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings;
    m_CorrAnsForLearned = settings.value("corr_answers", 10).toInt();

    if (settings.value("save_pos").toString().isEmpty())
        settings.setValue("save_pos", true);

    if (settings.value("save_pos").toBool()) {
        QPoint pos = settings.value("pos", QPoint(kDefaultXPosition,
                                                  kDefaultYPosition)).toPoint();
        QSize size = settings.value("size", QSize(kDefaultWidth,
                                                  kDefaultHeight)).toSize();
        restoreState(settings.value("window_state").toByteArray());
        resize(size);
        move(pos);
        bool isMax = settings.value("maximized", false).toBool();
        isMax ? showMaximized() : showNormal();
    } else {
        resize(kDefaultWidth, kDefaultHeight);
        move(kDefaultXPosition, kDefaultYPosition);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings;
    if (settings.value("save_pos").toBool()) {
        settings.setValue("pos", pos());

        if (isMaximized())
            settings.setValue("size", QSize(kDefaultWidth, kDefaultHeight));
        else
            settings.setValue("size", size());

        settings.setValue("maximized", isMaximized());
        settings.setValue("window_state", saveState());
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    m_CurrentFile = fileName;
    setWindowModified(false);

    QString shownName = m_CurrentFile;

    if (isFileOpened()) {
        if (m_CurrentFile.isEmpty())
            shownName = "untitled.wsf";

        setWindowTitle(shownName + "[*]" + " \u2014 "
                       + qApp->applicationName());
    } else {
        setWindowTitle(qApp->applicationName());
    }
}

void MainWindow::loadFile(const QString &fileName, bool import)
{
    XmlReader reader;

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
            m_Cards.append(reader.getNewSet());
            statusBar()->showMessage(tr("File imported"), 2000);
        } else {
            m_Cards = reader.getNewSet();
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("File loaded"), 2000);
        }

        tableWords->show();
        txtSearch->clear();

        updateTable(m_Cards);
        editActionsState();
    } else {
        QMessageBox::critical(this, tr("Error"), reader.getErrorMessage(),
                              QMessageBox::Ok);
    }
}

bool MainWindow::saveFile(const QString & fileName)
{
    XmlWriter writer(m_Cards);

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

void MainWindow::updateTags()
{
    WordsSet words;

    m_Tags.clear();

    if (m_Filtering && !txtSearch->text().isEmpty()) {
        SearchWordFilter filter;
        words = ptrsToWordsSet(filter.filter(txtSearch->text(),
                                              getPointersSet()));
    } else {
        words = m_Cards;
    }

    foreach (WordCard card, words) {
        m_Tags.unite(card.tags());
    }

    QString currTag = cmbTags->itemData(cmbTags->currentIndex()).toString();

    disconnect(cmbTags, SIGNAL(currentIndexChanged(int)), this, SLOT(tagFilter(int)));
    cmbTags->clear();
    cmbTags->addItem(tr("All"));

    foreach (QString tag, m_Tags) {
        int count = 0;

        foreach (WordCard word, words) {
            if (word.tags().contains(tag)) {
                count++;
            }
        }

        QString item = tag + QString(" (%1)").arg(count);

        cmbTags->addItem(QIcon(), item, tag);
    }

    for (int i = 0; i < cmbTags->count(); i++) {
        if (cmbTags->itemData(i).toString() == currTag) {
            cmbTags->setCurrentIndex(i);
            break;
        }
    }

    connect(cmbTags, SIGNAL(currentIndexChanged(int)), SLOT(tagFilter(int)));
}

void MainWindow::updateTable(const WordsSet &words)
{
    if (!m_Filtering) {
        m_FilteredSet = getPointersSet();
    }

    tableWords->clearContents();
    tableWords->setRowCount(0);

    int rowCount;
    QTableWidgetItem* tmp;

    foreach (WordCard card, words) {
        // Old row count and index of last row
        rowCount = tableWords->rowCount();

        tableWords->insertRow(rowCount);

        // Word
        tmp = new QTableWidgetItem(card.word());
        tableWords->setItem(rowCount, 0, tmp);

        // Transcription
        tmp = new QTableWidgetItem(card.transcription());
        tableWords->setItem(rowCount, 1, tmp);

        // Translation
        tmp = new QTableWidgetItem(card.translation());
        tableWords->setItem(rowCount, 2, tmp);

        // Learning progress in %
        int progress = (static_cast<double>(card.correctAnswers())
                            / m_CorrAnsForLearned * 100);
        progress = (progress > 100) ? 100 : progress;

        tmp = new QTableWidgetItem(QString("%1%").arg(progress));

        tmp->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWords->setItem(rowCount, 3, tmp);
    }

    tableWords->setCurrentCell(0, 0);

    updateTags();
}

bool MainWindow::isFileOpened()
{
    return (m_CurrentFile != "---");
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
    txtSearch->setEnabled(state);
    cmbTags->setEnabled(state);
}

void MainWindow::showCard(int index)
{
    if (isInRange(index)) {
        ViewCardDialog *viewDlg;
        if (m_Filtering)
            viewDlg = new ViewCardDialog(m_FilteredSet, this);
        else
            viewDlg = new ViewCardDialog(getPointersSet(), this);

        viewDlg->setCurrentCard(index);

        viewDlg->exec();

        if (viewDlg->isModified()) {
            setWindowModified(true);
            if (m_Filtering)
                updateTable(ptrsToWordsSet(m_FilteredSet));
            else
                updateTable(m_Cards);
        }

        delete viewDlg;

        tableWords->setCurrentCell(index, 0);
    }
}

bool MainWindow::isInRange(int curr)
{
    return ((curr >= 0) && (curr < m_Cards.size()));
}

WordsPtrSet MainWindow::getPointersSet()
{
    WordsPtrSet tmp;
    for (int i = 0; i < m_Cards.size(); i++)
        tmp.push_back(&m_Cards[i]);

    return tmp;
}

WordsSet MainWindow::ptrsToWordsSet(const WordsPtrSet &ptrs)
{
    WordsSet tmp;

    foreach (WordCard *card, ptrs) {
        tmp.push_back(*card);
    }

    return tmp;
}
