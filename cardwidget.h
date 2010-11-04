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

#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include "wordscard.h"

class QTextEdit;

/*
**  Widget for displaying contents of the card. Has two 'sides' - 'face' with the
**  word and it's transcription, and 'back' with the full info about the word
*/
class CardWidget : public QWidget
{
    Q_OBJECT

public:
    enum CardSide { Face = 0, Back };

    explicit CardWidget(QWidget *parent = 0);

    // showForeign - if true, shows 'word', 'plural', 'transcription' etc on
    // the back side
    void showWord(const WordCard& card, CardSide side = Face,
                  bool showForeign = true);
    void showWord(CardSide side = Face, bool showForeign = true);

public slots:
    // Shows other side of the card
    void showOtherSide();

private:
    static const int defaultWidth = 370;
    static const int defauldHeight = 265;

    // Get CSS from resource
    QString getCSS();
    void showFace(const WordCard& card);

    // showForeign - if true, shows 'word', 'plural', 'transcription' etc
    void showBack(const WordCard& card, bool showForeign);

    CardSide mSide;
    WordCard mWord;

    // Displayed content
    QString mHtmlCard;

    QTextEdit *txtCard;
};

#endif // CARDWIDGET_H
