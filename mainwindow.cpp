#include "mainwindow.h"
#include "neweditcarddialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolbars();
    createStatusBar();
    createTableWidget();

    setCurrentFile("---");
    setUnifiedTitleAndToolBarOnMac(true);

    readSettings();

    setAutoEditActionsState();
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

/* slot */
void MainWindow::newSet()
{
    if (maybeSave()) {
        setCurrentFile("");
        setWindowModified(true);
        mCards.clear();
        tableWords->show();
        updateTable();
        setAutoEditActionsState();
    }
}

/* slot */
void MainWindow::openSet()
{
    if (maybeSave()) {
        QString fileName =
                QFileDialog::getOpenFileName(this, tr("Open set"), "",
                                             tr("Words set file (*.wsf);;"
                                                "XML files (*.xml);;"
                                                "All files (*.*)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

/* slot */
bool MainWindow::saveSet()
{
    if (mCurFile.isEmpty()) {
        return saveSetAs();
    } else {
        return saveFile(mCurFile);
    }
}

/* slot */
bool MainWindow::saveSetAs()
{
    QString fileName =
            QFileDialog::getSaveFileName(this,tr("Save set"), "",
                                         tr("Words set file (*.wsf);;"
                                            "XML files (*.xml);;"
                                            "All files (*.*)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

/* slot */
void MainWindow::addCard()
{
    NewEditCardDialog dlg;
    if(dlg.exec()) {
        mCards.push_back(dlg.getNewCard());
        setWindowModified(true);
        updateTable();
    }
}

/* slot */
void MainWindow::editCard()
{
    int curr = tableWords->currentRow();
    if (!isInRange(curr)) {
        return;
    }
    NewEditCardDialog* dlg = new NewEditCardDialog(mCards.at(curr));
    if (dlg->exec()) {
        mCards.replace(curr, dlg->getNewCard());
        setWindowModified(true);
        updateTable();
    }
    delete dlg;
}

/* slot */
void MainWindow::viewCard() {
    showCard(tableWords->currentRow());
}

/* slot */
void MainWindow::deleteCard()
{
    int curr = tableWords->currentRow();
    if (!isInRange(curr)) {
        return;
    }
    mCards.removeAt(curr);
    setWindowModified(true);
    updateTable();
    tableWords->setCurrentCell(curr - 1, 0);
}

/* slot */
void MainWindow::importSet()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Import set"), "",
                                         tr("Words set file (*.wsf);;"
                                            "XML files (*.xml);;"
                                            "All files (*.*)"));
    if (!fileName.isEmpty()) {
        loadFile(fileName, true);
    }
    setWindowModified(true);
}

/* slot */
void MainWindow::startTraining()
{
    showCard(0);
}

/* slot */
void MainWindow::startQuiz()
{
    StartQuizDialog* dlg = new StartQuizDialog(&mCards);
    if (dlg->exec()) {
        QuizDialog *quizDlg = new QuizDialog(dlg->getWords(), dlg->getMode());
        quizDlg->exec();
        delete quizDlg;
    }
    delete dlg;
}

/* slot */
void MainWindow::about()
{

}

void MainWindow::createTableWidget() {
    tableWords = new QTableWidget();

    tableWords->setColumnCount(4);
    tableWords->setRowCount(0);

    QStringList headerLabels;
    headerLabels << "Word" << "Transcription" << "Translation" << "Learned";
    tableWords->setHorizontalHeaderLabels(headerLabels);

    tableWords->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWords->setSelectionMode(QAbstractItemView::SingleSelection);

    tableWords->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    tableWords->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    tableWords->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    tableWords->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);

    tableWords->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWords->installEventFilter(this);
    connect(tableWords, SIGNAL(doubleClicked(QModelIndex)), SLOT(viewCard()));

    tableWords->hide();
    setCentralWidget(tableWords);
}

void MainWindow::createActions()
{
    actionNewSet = new QAction(QIcon(":/icons/new.png"),
                               tr("&New set..."), this);
    actionNewSet->setShortcuts(QKeySequence::New);
    actionNewSet->setStatusTip(tr("Create a new card set"));
    connect(actionNewSet, SIGNAL(triggered()), this, SLOT(newSet()));

    actionOpenSet = new QAction(QIcon(":/icons/open.png"),
                                tr("&Open set..."), this);
    actionOpenSet->setShortcuts(QKeySequence::Open);
    actionOpenSet->setStatusTip(tr("Open an existing card set"));
    connect(actionOpenSet, SIGNAL(triggered()), this, SLOT(openSet()));

    actionSaveSet = new QAction(QIcon(":/icons/save.png"),
                                tr("&Save set"), this);
    actionSaveSet->setShortcuts(QKeySequence::Save);
    actionSaveSet->setStatusTip(tr("Save the card set to disk"));
    connect(actionSaveSet, SIGNAL(triggered()), this, SLOT(saveSet()));

    actionSaveSetAs = new QAction(tr("Save set &As..."), this);
    actionSaveSetAs->setShortcuts(QKeySequence::SaveAs);
    actionSaveSetAs->setStatusTip(tr("Save the card set under a new name"));
    connect(actionSaveSetAs, SIGNAL(triggered()), this, SLOT(saveSetAs()));

    actionQuit = new QAction(QIcon(":/icons/quit.png"), tr("&Quit"), this);
    actionQuit->setShortcuts(QKeySequence::Quit);
    actionQuit->setStatusTip(tr("Quit the application"));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    actionAddCard = new QAction(QIcon(":/icons/add.png"),
                                tr("&Add card..."), this);
    actionAddCard->setStatusTip(tr("Add a new word card to the set"));
    connect(actionAddCard, SIGNAL(triggered()), this, SLOT(addCard()));

    actionViewCard = new QAction(QIcon(":/icons/view.png"),
                                 tr("&View card..."), this);
    actionViewCard->setStatusTip(tr("View current word card"));
    connect(actionViewCard, SIGNAL(triggered()), this, SLOT(viewCard()));

    actionEditCard = new QAction(QIcon(":/icons/edit.png"),
                                 tr("&Edit card..."), this);
    actionEditCard->setStatusTip(tr("Edit current word card"));
    connect(actionEditCard, SIGNAL(triggered()), this, SLOT(editCard()));

    actionDeleteCard = new QAction(QIcon(":/icons/remove.png"),
                                   tr("&Delete card"), this);
    actionDeleteCard->setShortcuts(QKeySequence::Delete);
    actionDeleteCard->setStatusTip(tr("Delete the word card from the set"));
    connect(actionDeleteCard, SIGNAL(triggered()), this, SLOT(deleteCard()));

    actionImportSet = new QAction(tr("&Import set..."), this);
    actionImportSet->setStatusTip(
            tr("Append an existing set to the current"));
    connect(actionImportSet, SIGNAL(triggered()), this, SLOT(importSet()));

    //  actionBaseProperties;
    //  actionPreferences;

    actionStartTraining = new QAction(tr("Start &training..."), this);
    actionStartTraining->setStatusTip(tr("Start learning words from the set"));
    connect(actionStartTraining, SIGNAL(triggered()),
            this, SLOT(startTraining()));

    actionStartQuiz = new QAction(QIcon(":/icons/start_quiz.png"),
                                  tr("Start qui&z..."), this);
    actionStartQuiz->setStatusTip(
            tr("Test your knowledge of words from the set"));
    connect(actionStartQuiz, SIGNAL(triggered()),
            this, SLOT(startQuiz()));

    actionAbout = new QAction(tr("&About"), this);
    actionAbout->setStatusTip(tr("Show the application's About box"));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    actionAboutQt = new QAction(tr("About &Qt"), this);
    actionAboutQt->setStatusTip(tr("Show the Qt library's About box"));
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

#ifdef Q_OS_LINUX
    actionNewSet->setIcon(QIcon::fromTheme("document-new"));
    actionOpenSet->setIcon(QIcon::fromTheme("document-open"));
    actionSaveSet->setIcon(QIcon::fromTheme("document-save"));
    actionQuit->setIcon(QIcon::fromTheme("application-exit"));
    actionAddCard->setIcon(QIcon::fromTheme("list-add"));
    actionViewCard->setIcon(QIcon::fromTheme("edit-select-all"));
    actionEditCard->setIcon(QIcon::fromTheme("gtk-edit"));
    actionDeleteCard->setIcon(QIcon::fromTheme("list-remove"));
    actionStartQuiz->setIcon(QIcon::fromTheme("media-playback-start"));
#endif
}

void MainWindow::createMenus()
{
    menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionNewSet);
    menuFile->addAction(actionOpenSet);
    menuFile->addAction(actionSaveSet);
    menuFile->addAction(actionSaveSetAs);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);

    menuEdit = menuBar()->addMenu(tr("&Edit"));
    menuEdit->addAction(actionAddCard);
    menuEdit->addAction(actionViewCard);
    menuEdit->addAction(actionEditCard);
    menuEdit->addAction(actionDeleteCard);
    menuEdit->addAction(actionImportSet);
    //  menuEdit->addAction(actionBaseProperties);
    //  menuEdit->addAction(actionPreferences);

    menuTraining = menuBar()->addMenu(tr("&Training"));
    menuTraining->addAction(actionStartTraining);
    menuTraining->addAction(actionStartQuiz);

    menuBar()->addSeparator();

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


    //  toolBar->addAction(actionBaseProperties);
    toolBar->addAction(actionStartQuiz);
    toolBar->addSeparator();
    //  toolBar->addAction(actionPreferences);
    toolBar->addAction(actionQuit);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    bool isMax = settings.value("maximized", false).toBool();
    resize(size);
    move(pos);
    isMax ? showMaximized() : showNormal();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("pos", pos());
    if (!isMaximized()) {
        settings.setValue("size", size());
    } else {
        settings.setValue("size", QSize(800, 600));
    }
    settings.setValue("maximized", isMaximized());
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    mCurFile = fileName;
    setWindowModified(false);

    QString shownName = mCurFile;
    if (!isFileOpened()) {
        setWindowTitle(qApp->applicationName());
    } else {
        if (mCurFile.isEmpty()) {
            shownName = "untitled.wsf";
        }

        setWindowTitle(shownName + "[*]" + " \u2014 "
                       + qApp->applicationName());
    }
}

void MainWindow::loadFile(const QString &fileName, bool import)
{
    WordsSet temp;
    XmlReader reader(&temp);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    bool isError = reader.readFile(fileName);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (!isError) {
        QMessageBox::critical(this, tr("Error"), reader.getErrorMessage(),
                              QMessageBox::Ok);
    } else {
        if (import) {
            mCards.append(temp);
            statusBar()->showMessage(tr("File imported"), 2000);
        } else {
            mCards = temp;
            setCurrentFile(fileName);
            statusBar()->showMessage(tr("File loaded"), 2000);
        }
        tableWords->show();
        updateTable();
        setAutoEditActionsState();
    }
}

bool MainWindow::saveFile(const QString & fileName)
{
    XmlWriter writer(&mCards);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    bool isError = writer.writeFile(fileName);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (!isError) {
        QMessageBox::critical(this, tr("Error"), writer.getErrorMessage(),
                              QMessageBox::Ok);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);

    return true;
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

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateTable()
{
    tableWords->clearContents();
    tableWords->setRowCount(0);

    WordsSet::iterator it = mCards.begin();
    int rowCount;
    QTableWidgetItem* tmp;
    for ( ; it != mCards.end(); it++) {
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
        // Learned
        tmp = new QTableWidgetItem(it->isLearned()
                                   ? tr("Yes") : tr("No"));
        tmp->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWords->setItem(rowCount, 3, tmp);;
    }

    tableWords->setCurrentCell(0, 0);
}

bool MainWindow::isFileOpened()
{
    return (mCurFile != "---");
}

void MainWindow::setAutoEditActionsState()
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
}

void MainWindow::showCard(int index)
{
    if (!isInRange(index)) {
        return;
    }

    ViewCardDialog *viewDlg = new ViewCardDialog(&mCards);
    viewDlg->setCurrentWord(index);

    viewDlg->exec();

    if (viewDlg->isModified()) {
        setWindowModified(true);
        updateTable();
    }

    delete viewDlg;
}

bool MainWindow::isInRange(int curr)
{
    return ((curr >= 0) && (curr < mCards.size()));
}
