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

#ifndef VIEWCARDDIALOG_H
#define VIEWCARDDIALOG_H

#include <QDialog>
#include "wordcard.h"

class CardWidget;

// Class for viewing cards
class ViewCardDialog : public QDialog
{
    Q_OBJECT

public:
    ViewCardDialog(const WordsPtrSet & cards, QWidget *parent = 0);

    void setCurrentCard(int index);
    bool isModified();

private slots:
    void nextCard();
    void prevCard();
    void editCard();

private:
    //== Static constants ===============================================

    static const int kDefaultWidth = 400;
    static const int kDefaultHeight = 360;

    //===================================================================


    //== Private member functions =======================================

    void createInterface();

    // Switches Previous and Next buttons according to mCurrentCard
    void switchButtons();

    //===================================================================


    //== Private member variables =======================================

    bool m_Modified;
    int  m_CurrentCard;

    WordsPtrSet m_Cards;

    //===================================================================

    //== Private widgets and other QObjects =============================

    QPushButton *btnPrevious;
    QPushButton *btnTurn;
    QPushButton *btnNext;
    QPushButton *btnEdit;

    CardWidget  *wgtCard;

    //===================================================================
};

#endif // VIEWCARDDIALOG_H
