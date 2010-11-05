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

#include "cardwidget.h"

#include "wordscard.h"
#include <QLayout>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>

CardWidget::CardWidget(QWidget *parent) :
    QWidget(parent), mSide(Face)
{
    txtCard = new QTextEdit();
    txtCard->setReadOnly(true);

    QHBoxLayout* lt = new QHBoxLayout;
    lt->addWidget(txtCard);
    lt->setMargin(0);

    setLayout(lt);
    setMinimumSize(defaultWidth, defauldHeight);
}

void CardWidget::showCard(const WordCard& card, CardSide faceSide,
                          bool showForeign)
{
    mSide = faceSide;
    mCard = card;
    mHtmlCard.clear();

    // Attaching CSS
    mHtmlCard.append("<head><style type=\"text/css\">");
    mHtmlCard.append(getCSS());
    mHtmlCard.append("</style></head><body style=\"font-size:9pt;\">");

    if (mSide == Face)
        showFace(card);
    else
        showBack(card, showForeign);

    mHtmlCard.append("</body>");

    txtCard->setHtml(mHtmlCard);
}

void CardWidget::showCard(CardSide faceSide, bool showForeign)
{
    showCard(mCard, faceSide, showForeign);
}

void CardWidget::showOtherSide()
{
    mSide = (mSide == Face) ? Back : Face;
    showCard(mCard, mSide);
}

QString CardWidget::getCSS() {
    QFile file(":/style.css");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        return stream.readAll();
    }

    return QString();
}

void CardWidget::showFace(const WordCard& card)
{
    // SHIT: Trying to align this fucked up QTextEdit
    mHtmlCard.append("<br />");
    mHtmlCard.append("<br />");
    mHtmlCard.append("<br />");
    mHtmlCard.append("<br />");

    // Word and its gender
    mHtmlCard.append(QString("<p class=\"word\" align=\"center\"><b>%1</b>"
                            "<span style=\"font-size:12pt\"> %2</span></p>")
                    .arg(card.word(), card.genderShortString()));


    // Category
    mHtmlCard.append(QString("<p class=\"category\" align=\"center\">%1</p>")
                    .arg(card.lexCategoriesShortString()));

    // Transcription
    if (!card.transcription().isEmpty()) {
        mHtmlCard.append(QString("<p class=\"transcription\" align=\"center\">"
                                "[%1]</p>")
                        .arg(card.transcription()));
    }
}

void CardWidget::showBack(const WordCard& card, bool showForeign)
{
    if (showForeign) {
        // Word and its gender
        mHtmlCard.append(QString("<p class=\"word\" align=\"center\"><b>%1</b>"
                                "<span style=\"font-size:12pt\"> %2</span></p>")
                .arg(card.word(), card.genderShortString()));

        // Plural
        mHtmlCard.append(QString("<p class=\"plural\" align=\"center\">%1</p>")
                .arg(card.plural()));

        // Category
        mHtmlCard.append(QString("<p class=\"category\" align=\"center\">%1</p>")
                .arg(card.lexCategoriesShortString()));

        // Transcription
        if (!card.transcription().isEmpty()) {
            mHtmlCard.append(QString("<p class=\"transcription\" align=\"center\">"
                                    "[%1]</p>")
                    .arg(card.transcription()));
        }
    } else {
        // SHIT: Trying to align this fucked up QTextEdit
        mHtmlCard.append("<br />");
        mHtmlCard.append("<br />");
        mHtmlCard.append("<br />");
        mHtmlCard.append("<br />");
    }


    // Translation
    mHtmlCard.append(QString("<p class=\"translation\" align=\"center\">"
                            "<b>%1</b></p>")
            .arg(card.translation()));

    // Examples
    if (card.examplesSize()) {
        mHtmlCard.append(QString("<p class=\"examples\">%1</p>")
                        .arg(tr("Examples:")));
    }

    for (int i = 0; i < card.examplesSize(); i++) {
        if (showForeign) {
            mHtmlCard.append(QString("<p class=\"example\">%1. %2<br />— %3</p>")
                            .arg(i + 1).arg(card.exampleAt(i).first,
                                        card.exampleAt(i).second));
        } else {
            mHtmlCard.append(QString("<p class=\"example\">%1. %2</p>")
                            .arg(i + 1).arg(card.exampleAt(i).second));
        }

    }
}
