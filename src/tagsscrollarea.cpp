/******************************************************************************
** WordTrain 0.9.2 -- Foreign words trainer
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

#include <QLayout>
#include <QCheckBox>

#include "tagsscrollarea.h"

TagsScrollArea::TagsScrollArea(const Tags &tags, QWidget *parent) :
    QScrollArea(parent), m_Tags(tags)
{
    // Contains the layout with checkboxes
    QWidget *container = new QWidget;
    ltTags = new QVBoxLayout;
    container->setLayout(ltTags);

    setWidget(container);
    fillTags();

    setLayout(ltTags);
}

void TagsScrollArea::fillTags()
{
    foreach (QString tag, m_Tags) {
        QCheckBox *ch = new QCheckBox(tag);
        connect(ch, SIGNAL(clicked()), SLOT(emitStateChanged()));
        ltTags->addWidget(ch);
    }

    ltTags->addStretch(1);
}

void TagsScrollArea::emitStateChanged()
{
    Tags now = selectedTags();
    bool isNowChecked = (now.size() > m_Selected.size());
    Tags justSelected = (isNowChecked) ? now - m_Selected
                                       : m_Selected - now;
    m_Selected = now;
    emit stateChanged(justSelected.values().at(0), isNowChecked);
}

Tags TagsScrollArea::selectedTags()
{
    Tags tmp;
    for (int i = 0, size = m_Tags.size(); i < size; i++) {
        if (chckBoxClickedAt(i))
            tmp << chckBoxTextAt(i);
    }

    return tmp;
}

QString TagsScrollArea::chckBoxTextAt(int index)
{
    QCheckBox *tmp = qobject_cast<QCheckBox*>(ltTags->itemAt(index)->widget());
    return tmp->text();
}

bool TagsScrollArea::chckBoxClickedAt(int index)
{
    QCheckBox *tmp = qobject_cast<QCheckBox*>(ltTags->itemAt(index)->widget());
    return tmp->isChecked();
}

