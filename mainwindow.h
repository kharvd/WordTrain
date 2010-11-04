/******************************************************************************
** WordTrain 0.8.5 -- Foreign words trainer
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wordscard.h"

class QWidget;
class QObject;
class QString;
class QEvent;
class QCloseEvent;
class QTableWidget;
class QAction;
class QMenu;
class QMenuBar;

// Main window class.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    // On close
    void closeEvent(QCloseEvent *event);

    // For handling context menu event on tableWords
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void newSet();
    void openSet();
    bool saveSet();
    bool saveSetAs();
    void addCard();
    void viewCard();
    void editCard();
    void deleteCard();
    void importSet();
    void settings();
    void startTraining();
    void startQuiz();
    void about();

private:
    // Some constants
    static const int defaultWidth = 640;
    static const int defaultHeight = 480;
    static const int defaultXPosition = 200;
    static const int defaultYPosition = 200;

    // Creating interface
    void createTableWidget();
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolbars();
    void createStatusBar();
    void createInterface();

    void readSettings();
    void writeSettings();

    /* Checks whether the file should be saved or not, saves it if it should,
    returns false if user rejected to close file, and true if we can
    close it. */
    bool maybeSave();

    // If import is true, we append existing file to current
    void loadFile(const QString & fileName, bool import = false);
    bool saveFile(const QString & fileName);
    void setCurrentFile(const QString &fileName);
    bool isFileOpened();

    // Fills the table with words
    void updateTable();

    // Shows card with index 'index'
    void showCard(int index);

    // Checks whether 'curr' is not out of range of mCards
    bool isInRange(int curr);

    // If set is not opened or is empty, disable all edit actions
    void setAutoEditActionsState();

    // Returns set of the pointers to all words in the main set
    WordsPtrSet getPointersSet();

    // Word is learned after this number of correct answers
    int corrAnsForLearned;

    //=============================================================

    // All the words of the current set.
    WordsSet mCards;
    // Path to current file
    QString mCurrentFile;

    // Main widget - table with words of the set
    QTableWidget *tableWords;

    // Actions
    QAction *actionNewSet;
    QAction *actionOpenSet;
    QAction *actionSaveSet;
    QAction *actionSaveSetAs;
    QAction *actionQuit;
    QAction *actionAddCard;
    QAction *actionViewCard;
    QAction *actionEditCard;
    QAction *actionDeleteCard;
    QAction *actionImportSet;
    QAction *actionSettings;
    QAction *actionStartTraining;
    QAction *actionStartQuiz;
    QAction *actionAbout;
    QAction *actionAboutQt;

    // Menus
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTraining;
    QMenu *menuAbout;

    // Main menu
    QMenuBar *menubar;

    // Main toolbar
    QToolBar *toolBar;
};

#endif // MAINWINDOW_H
