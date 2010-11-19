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
#ifndef EXAMPLESWIDGET_H
#define EXAMPLESWIDGET_H

#include <QWidget>
#include "wordscard.h"

class QFormLayout;

// Widget for entering examples of using words in NewEditDialog
class ExamplesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExamplesWidget(QWidget *parent = 0);

    // Sets contents of example at index to example
    void setExampleAt(int index, Example example);

    // Returns example at index
    Example getExampleAt(int index);

    // Returns number of examples in the widget
    int examplesCount();

    // Returns true if you can add an example to the widget
    bool canAdd();

    // Maximum number of examples in the widget
    const static int maxExamples = 50;

public slots:
    void addExample();

private:
    QFormLayout *ltExamples;
};

#endif // EXAMPLESWIDGET_H
