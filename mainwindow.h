/******************************************************************************
** WordTrain 0.8.4 -- Foreign words trainer
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

#include <QtGui>
#include "xmlreader.h"
#include "xmlwriter.h"
#include "wordscard.h"
#include "viewcarddialog.h"
#include "startquizdialog.h"
#include "quizdialog.h"
#include "neweditcarddialog.h"
#include "settingsdialog.h"
#include "aboutdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
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
//  void baseProperties();
    void preferences();
    void startTraining();
    void startQuiz();
    void about();

private:
    void createTableWidget();
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolbars();
    void createStatusBar();
    void createInterface();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    /* If import is true, we append existing file to current */
    void loadFile(const QString & fileName, bool import = false);
    bool saveFile(const QString & fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString & fullFileName);
    void clearCards();
    bool isFileOpened();
    void updateTable();
    void showCard(int index);

    WordsPtrSet getPointersSet();

    /* Checks whether 'curr' is not out of range of mCards */
    bool isInRange(int curr);

    /* If set is not opened or is empty, disable all edit actions */
    void setAutoEditActionsState();

    int corrAnsForLearned;

    WordsSet mCards;
    QString mCurFile;

    QTableWidget *tableWords;

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
//  QAction *actionBaseProperties;
    QAction *actionPreferences;
    QAction *actionStartTraining;
    QAction *actionStartQuiz;
    QAction *actionAbout;
    QAction *actionAboutQt;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTraining;
    QMenu *menuAbout;
    QMenuBar *menubar;
    QToolBar *toolBar;
};

#endif // MAINWINDOW_H
