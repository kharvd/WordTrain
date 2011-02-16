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

#include "xmlwriter.h"

XmlWriter::XmlWriter(const WordsSet &cards)
{
    m_Cards = cards;
    m_Writer.setAutoFormatting(true);
}

bool XmlWriter::writeFile(const QString & fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        m_ErrorMessage = QString("Error: Cannot open file ")
                        + qPrintable(fileName)
                        + ": " + qPrintable(file.errorString());
        return false;
    }

    m_Writer.setDevice(&file);

    m_Writer.writeStartDocument();
    m_Writer.writeStartElement("wordsset");

    foreach (WordCard card, m_Cards)
        writeCard(card);

    m_Writer.writeEndElement();

    return true;
}

void XmlWriter::writeCard(const WordCard &card) {
    m_Writer.writeStartElement("wordcard");

    m_Writer.writeEmptyElement("word");
    m_Writer.writeAttribute("value", card.word());

    m_Writer.writeEmptyElement("transcription");
    m_Writer.writeAttribute("value", card.transcription());

    m_Writer.writeEmptyElement("translation");
    m_Writer.writeAttribute("value", card.translation());

    m_Writer.writeEmptyElement("plural");
    m_Writer.writeAttribute("value", card.plural());

    m_Writer.writeEmptyElement("gender");
    m_Writer.writeAttribute("value", QString::number(card.gender()));

    m_Writer.writeEmptyElement("category");
    m_Writer.writeAttribute("value", QString::number(card.category()));

    m_Writer.writeEmptyElement("answers");
    m_Writer.writeAttribute("value", QString::number(card.correctAnswers()));

    foreach (Example ex, card.examples())
        writeExample(ex);

    foreach (QString tag, card.tags()) {
        m_Writer.writeEmptyElement("tag");
        m_Writer.writeAttribute("value", tag);
    }

    m_Writer.writeEndElement();
}

void XmlWriter::writeExample(const Example &ex) {
    m_Writer.writeStartElement("example");
    m_Writer.writeTextElement("ex", ex.first);
    m_Writer.writeTextElement("tr", ex.second);
    m_Writer.writeEndElement();
}

QString XmlWriter::getErrorMessage() {
    return m_ErrorMessage;
}
