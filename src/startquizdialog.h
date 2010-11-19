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

#ifndef STARTQUIZDIALOG_H
#define STARTQUIZDIALOG_H

#include <QDialog>
#include "wordscard.h"
#include "quizdialog.h"

class QButtonGroup;
class QRadioButton;
class QCheckBox;
class QLineEdit;

// Dialog for selecting quiz options and starting the quiz
class StartQuizDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartQuizDialog(WordsSet * words, QWidget *parent = 0);

    // Returns chosen words
    WordsPtrSet getCards();

    // Returns selected choice mode
    ChoiceMode getChoiceMode();

    // Returns selected hiding mode
    HideMode getHideMode();

private slots:
    /* Enables or disables line edit with number of words depending on
     chckAllWords */
    void toggleTxtNumWords(bool disable);

private:
    void createInterface();

    QButtonGroup *grpChoiceMode;
    QButtonGroup *grpHideMode;
    QRadioButton *radioMultiChoiceMode;
    QRadioButton *radioNoChoiceMode;
    QRadioButton *radioRandomOrder;
    QRadioButton *radioListOrder;
    QRadioButton *radioWordHide;
    QRadioButton *radioTranslationHide;
    QRadioButton *radioRandomHide;
    QCheckBox *chckIncLearned;
    QCheckBox *chckAllWords;
    QLineEdit *txtNumWords;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    WordsPtrSet mCards;
};

#endif // STARTQUIZDIALOG_H
