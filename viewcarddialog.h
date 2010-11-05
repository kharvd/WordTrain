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

#ifndef VIEWCARDDIALOG_H
#define VIEWCARDDIALOG_H

#include <QDialog>
#include "wordscard.h"

class CardWidget;

// Class for viewing cards
class ViewCardDialog : public QDialog
{
    Q_OBJECT

public:
    // This class should be able to edit contents of the cards
    ViewCardDialog(const WordsPtrSet & cards, QWidget *parent = 0);
    ViewCardDialog(WordsSet *cards, QWidget *parent = 0);

    void setCurrentCard(int index);
    bool isModified();

private slots:
    void nextCard();
    void prevCard();
    void editCard();

private:
    static const int defaultWidth = 400;
    static const int defaultHeight = 360;

    void createInterface();

    // Switches Previous and Next buttons according to mCurrentCard
    void switchButtons();

    QPushButton *btnPrevious;
    QPushButton *btnTurn;
    QPushButton *btnNext;
    QPushButton *btnEdit;
    CardWidget *cardText;

    bool mModified;
    int mCurrentCard;
    WordsPtrSet mCards;
};

#endif // VIEWCARDDIALOG_H
