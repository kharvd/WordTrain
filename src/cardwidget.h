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
    enum CardElement {
        Nothing = 0,
        Word = 0x1,
        Transcription = 0x2,
        Translation = 0x4,
        Category = 0x8,
        Gender = 0x10,
        Plural = 0x20,
        Examples = 0x40,
        ExamplesTranslation = 0x80,

        FaceDefault = Word | Transcription | Category | Gender,
        BackNoForeign = Translation | ExamplesTranslation,
        BackAll = Translation | ExamplesTranslation | Word | Transcription
                  | Category | Gender | Plural | Examples
    };

    Q_DECLARE_FLAGS(CardElements, CardElement)

    enum CardSide {
        FaceSide = 0,
        BackSide
    };

    explicit CardWidget(QWidget *parent = 0);

    void showCard(const WordCard& card, CardSide side = FaceSide,
                  CardElements elements = BackAll);
    void showCard(CardSide side = FaceSide, CardElements elements = BackAll);

public slots:
    // Shows other side of the card
    void showOtherSide(CardElements elements = BackAll);

private:
    static const int kDefaultWidth = 370;
    static const int kDefaultHeight = 265;

    // Get CSS from resource
    QString getCSS();
    void showFace(const WordCard& card, CardElements elements);

    // showForeign - if true, shows 'word', 'plural', 'transcription' etc
    void showBack(const WordCard& card, CardElements elements);

    CardSide m_Side;
    WordCard m_Card;

    // Displayed content
    QString m_CardHtml;

    QTextEdit *txtCard;
};

#endif // CARDWIDGET_H
