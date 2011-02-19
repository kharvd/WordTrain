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

#ifndef TAGSSCROLLAREA_H
#define TAGSSCROLLAREA_H

#include <QScrollArea>
#include "wordcard.h"

class QVBoxLayout;
class QCheckBox;

// Displays checkboxes in scrollarea for tags. Used in QuizWordChooserDialog
class TagsScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    explicit TagsScrollArea(const Tags &tags, QWidget *parent = 0);
    virtual ~TagsScrollArea() { }

    Tags selectedTags();

signals:
    // Emitted when one of the checkboxes has been clicked
    // First parameter is the name of the selected tag, and second is its
    // current state
    void stateChanged(const QString&, bool);

private slots:
    void emitStateChanged();

private:
    //== Private member functions =======================================

    QString chckBoxTextAt(int index);
    bool    chckBoxClickedAt(int index);

    void fillTags();

    //===================================================================


    //== Private member variables =======================================

    Tags m_Tags;
    Tags m_Selected; // Previously selected tags

    //===================================================================


    //== Private widgets and other QObjects =============================

    QScrollArea *scrlArea;
    QVBoxLayout *ltTags;

    //===================================================================
};

#endif // TAGSSCROLLAREA_H
