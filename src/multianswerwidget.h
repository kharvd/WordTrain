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

#ifndef MULTIANSWERWIDGET_H
#define MULTIANSWERWIDGET_H

#include "answerwidget.h"

class QButtonGroup;
class QVBoxLayout;

// Widget for answering with multichoice
class MultiAnswerWidget : public AnswerWidget
{
    Q_OBJECT
public:
    explicit MultiAnswerWidget(QWidget *parent = 0);

    // Returns user's answer
    virtual QString getAnswer();

    // Sets choices
    virtual void setAnswers(const QStringList & answers);

    // Clears widget
    virtual void clear();

private:
    // Creates radio buttons with answers
    void createRadios();

protected:
    QButtonGroup *grpAnswers;
    QVBoxLayout *ltRadios;
    QStringList mAnswers;
};

#endif // MULTIANSWERWIDGET_H
