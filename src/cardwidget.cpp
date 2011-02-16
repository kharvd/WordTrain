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

#include "cardwidget.h"

#include "wordcard.h"
#include <QLayout>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>

Q_DECLARE_OPERATORS_FOR_FLAGS(CardWidget::CardElements)

CardWidget::CardWidget(QWidget *parent) :
    QWidget(parent), m_Side(FaceSide)
{
    txtCard = new QTextEdit();
    txtCard->setReadOnly(true);

    QHBoxLayout* lt = new QHBoxLayout;
    lt->addWidget(txtCard);
    lt->setMargin(0);

    setLayout(lt);
    setMinimumSize(kDefaultWidth, kDefaultHeight);
}

void CardWidget::showCard(const WordCard& card, CardSide faceSide,
                          CardElements elements)
{
    m_Side = faceSide;
    m_Card = card;
    m_CardHtml.clear();

    // Attaching CSS
    m_CardHtml.append("<head><style type=\"text/css\">");
    m_CardHtml.append(getCSS());
    m_CardHtml.append("</style></head><body style=\"font-size:9pt;\">");

    if (m_Side == FaceSide)
        showFace(card, elements);
    else
        showBack(card, elements);

    m_CardHtml.append("</body>");

    txtCard->setHtml(m_CardHtml);
}

void CardWidget::showCard(CardSide faceSide, CardElements elements)
{
    showCard(m_Card, faceSide, elements);
}

void CardWidget::showOtherSide(CardElements elements)
{
    m_Side = (m_Side == FaceSide) ? BackSide : FaceSide;
    showCard(m_Card, m_Side, elements);
}

QString CardWidget::getCSS() {
    QFile file(":/style.css");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        return stream.readAll();
    }

    return QString();
}

void CardWidget::showFace(const WordCard& card, CardElements elements)
{
    // Get flags
    bool word = elements & CardWidget::Word;
    bool gender = elements & CardWidget::Gender;
    bool category = elements & CardWidget::Category;
    bool transcription = elements & CardWidget::Transcription;

    // SHIT: Trying to align this fucked up QTextEdit
    m_CardHtml.append("<br />");
    m_CardHtml.append("<br />");
    m_CardHtml.append("<br />");
    m_CardHtml.append("<br />");

    if (word) {
        // Word
        m_CardHtml.append(QString("<p class=\"word\" align=\"center\"><b>%1</b>")
                        .arg(card.word()));
    }

    if (gender) {
        // Gender
        m_CardHtml.append(QString("<span style=\"font-size:12pt\"> %1</span></p>")
                        .arg(card.genderShortString()));
    } else if (word) {
        m_CardHtml.append(QString("</p>")
                        .arg(card.genderShortString()));
    }



    if (category) {
        // Category
        m_CardHtml.append(QString("<p class=\"category\" align=\"center\">%1</p>")
                        .arg(card.lexCategoriesShortString()));
    }

    if (transcription && !card.transcription().isEmpty()) {
        m_CardHtml.append(QString("<p class=\"transcription\" align=\"center\">"
                                 "[%1]</p>")
                         .arg(card.transcription()));
    }
}

void CardWidget::showBack(const WordCard& card, CardElements elements)
{
    // Get flags
    bool word = elements & CardWidget::Word;
    bool gender = elements & CardWidget::Gender;
    bool category = elements & CardWidget::Category;
    bool transcription = elements & CardWidget::Transcription;
    bool plural = elements & CardWidget::Plural;
    bool translation = elements & CardWidget::Translation;
    bool examples = elements & CardWidget::Examples;
    bool examplesTranslation = elements & CardWidget::ExamplesTranslation;

    if (word) {
        // Word
        m_CardHtml.append(QString("<p class=\"word\" align=\"center\"><b>%1</b>")
                        .arg(card.word()));
    }

    if (gender) {
        // Gender
        m_CardHtml.append(QString("<span style=\"font-size:12pt\"> %1</span></p>")
                        .arg(card.genderShortString()));
    } else if (word) {
        m_CardHtml.append(QString("</p>")
                        .arg(card.genderShortString()));
    }

    if (plural) {
        // Plural
        m_CardHtml.append(QString("<p class=\"plural\" align=\"center\">%1</p>")
                .arg(card.plural()));
    }

    if (category) {
        // Category
        m_CardHtml.append(QString("<p class=\"category\" align=\"center\">%1</p>")
                .arg(card.lexCategoriesShortString()));
    }

    if (transcription && !card.transcription().isEmpty()) {
        m_CardHtml.append(QString("<p class=\"transcription\" align=\"center\">"
                                 "[%1]</p>")
                         .arg(card.transcription()));
    }

    // If we don't want to show foreign words, align contents
    if (!word) {
        // SHIT: Trying to align this fucked up QTextEdit
        m_CardHtml.append("<br />");
        m_CardHtml.append("<br />");
        m_CardHtml.append("<br />");
        m_CardHtml.append("<br />");
    }

    if (translation) {
        // Translation
        m_CardHtml.append(QString("<p class=\"translation\" align=\"center\">"
                                "<b>%1</b></p>")
                .arg(card.translation()));
    }


    // Examples
    if (examples && card.examplesSize()) {
        m_CardHtml.append(QString("<p class=\"examples\">%1</p>")
                        .arg(tr("Examples:")));
    }

    if (examples || examplesTranslation) {
        for (int i = 0; i < card.examplesSize(); i++) {
            if (examples && examplesTranslation) {
                m_CardHtml.append(QString("<p class=\"example\">%1. %2<br />— %3</p>")
                                .arg(i + 1).arg(card.exampleAt(i).first,
                                            card.exampleAt(i).second));
            } else if (!examples && examplesTranslation) {
                m_CardHtml.append(QString("<p class=\"example\">%1. %2</p>")
                                .arg(i + 1).arg(card.exampleAt(i).second));
            } else if (examples && !examplesTranslation) {
                m_CardHtml.append(QString("<p class=\"example\">%1. %2</p>")
                                .arg(i + 1).arg(card.exampleAt(i).first));
            }
        }
    }
}
