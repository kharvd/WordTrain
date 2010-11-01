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

#include "exampleswidget.h"

ExamplesWidget::ExamplesWidget(QWidget *parent) :
        QWidget(parent)
{
    ltExamples = new QFormLayout;
    ltExamples->setAlignment(Qt::AlignTop);

    setLayout(ltExamples);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

int ExamplesWidget::examplesCount() {
    return (ltExamples->count() / 4);
}

bool ExamplesWidget::canAdd() {
    if (examplesCount() < maxExamples) {
        return true;
    }

    return false;
}

void ExamplesWidget::addExample() {
    int num = examplesCount() + 1;
    if(canAdd()) {
        QLineEdit* txt1 = new QLineEdit();
        QLineEdit* txt2 = new QLineEdit();

        ltExamples->addRow(tr("Example %1:").arg(num), txt1);
        ltExamples->addRow(tr("Translation %1:").arg(num), txt2);
    }
}

void ExamplesWidget::setExampleAt(int index, Example ex) {
    QLineEdit* example = qobject_cast<QLineEdit *>(ltExamples->itemAt
                                                   (index * 4 + 1)->widget());
    Q_ASSERT(example);

    QLineEdit* translation = qobject_cast<QLineEdit *>(ltExamples->itemAt
                                                       (index * 4 + 3)->widget());
    Q_ASSERT(translation);

    example->setText(ex.first);
    translation->setText(ex.second);
}

Example ExamplesWidget::getExampleAt(int index) {
    QLineEdit* example = qobject_cast<QLineEdit *>(ltExamples->itemAt
                                                   (index * 4 + 1)->widget());
    Q_ASSERT(example);

    QLineEdit* translation = qobject_cast<QLineEdit *>(ltExamples->itemAt
                                                       (index * 4 + 3)->widget());
    Q_ASSERT(translation);

    Example tmp;
    tmp.first = example->text();
    tmp.second = translation->text();
    return tmp;
}
