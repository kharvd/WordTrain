/******************************************************************************
** WordTrain 0.8.4 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov
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
******************************************************************************/

#ifndef STARTQUIZDIALOG_H
#define STARTQUIZDIALOG_H

#include <QtGui>
#include "wordschooser.h"
#include "wordscard.h"
#include "quizdialog.h"

class StartQuizDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartQuizDialog(WordsSet * words, QWidget *parent = 0);
    WordsPtrSet getWords();
    ChoiceMode getChoiceMode();
    HideMode getHideMode();

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
    QLineEdit *txtNumWords;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    WordsPtrSet mWords;
};

#endif // STARTQUIZDIALOG_H
