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

#include "xmlreader.h"

XmlReader::XmlReader(WordsSet *cards)
{
    m_Cards = cards;
}

bool XmlReader::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        m_ErrorMessage = QString("Error: Cannot read file ")
                        + qPrintable(fileName)
                        + ": " + qPrintable(file.errorString());
        return false;
    }

    m_Reader.setDevice(&file);

    m_Reader.readNext();
    skipWhitespaces();

    while (!m_Reader.atEnd()) {
        if (m_Reader.isStartElement()) {
            if (m_Reader.name() == "wordsset") {
                readWordsSet();
            } else {
                m_Reader.raiseError(QObject::tr("Not a words set file"));
            }
        } else {
            m_Reader.readNext();
            skipWhitespaces();
        }
    }

    file.close();
    if (m_Reader.hasError()) {
        m_ErrorMessage = QString("Error: Failed to parse file ")
                        + qPrintable(fileName) + ": "
                        + qPrintable(m_Reader.errorString());
        return false;
    } else if (file.error() != QFile::NoError) {
        m_ErrorMessage = QString("Error: Cannot read file ")
                        + qPrintable(fileName) + ": "
                        + qPrintable(file.errorString());
        return false;
    }
    return true;
}

void XmlReader::readWordsSet()
{
    m_Reader.readNext();
    skipWhitespaces();

    while (!m_Reader.atEnd() && !m_Reader.isEndElement()) {
        if (m_Reader.isStartElement()) {
            if (m_Reader.name() == "wordcard") {
                WordCard curr;
                m_Cards->push_back(curr);
                readWordCard();
            } else {
                skipUnknownElement();
            }
        } else {
            m_Reader.readNext();
            skipWhitespaces();
        }
    }

    m_Reader.readNext();
}

void XmlReader::readWordCard()
{
    WordCard & card = m_Cards->last();
    m_Reader.readNext();
    skipWhitespaces();

    while (!m_Reader.atEnd() && !(m_Reader.isEndElement()
        && m_Reader.name() == "wordcard"))
    {
        if (m_Reader.isStartElement()) {
            if (m_Reader.name() == "word") {
                card.setWord(m_Reader.attributes().value("value").toString());
                m_Reader.readNext();
                skipWhitespaces();
            } else if (m_Reader.name() == "transcription") {
                card.setTranscription(
                        m_Reader.attributes().value("value").toString());
                m_Reader.readNext();
                skipWhitespaces();
            } else if (m_Reader.name() == "translation") {
                card.setTranslation(
                        m_Reader.attributes().value("value").toString());
                m_Reader.readNext();
                skipWhitespaces();
            } else if (m_Reader.name() == "plural") {
                card.setPlural(
                        m_Reader.attributes().value("value").toString());
                m_Reader.readNext();
                skipWhitespaces();
            } else if (m_Reader.name() == "gender") {
                card.setGender(
                        m_Reader.attributes().value("value").toString().toInt());
                m_Reader.readNext();
                skipWhitespaces();
            } else if (m_Reader.name() == "category") {
                card.setCategory(
                        m_Reader.attributes().value("value").toString().toInt());
                m_Reader.readNext();
                skipWhitespaces();
            } else if (m_Reader.name() == "answers") {
                card.setCorrectAnswers(
                        m_Reader.attributes().value("value").toString().toInt());
                m_Reader.readNext();
                skipWhitespaces();
            } else if (m_Reader.name() == "example") {
                readExample();
            } else {
                skipUnknownElement();
            }
        } else {
            m_Reader.readNext();
            skipWhitespaces();
        }
    }

    m_Reader.readNext();
}

void XmlReader::readExample() {
    WordCard & card = m_Cards->last();

    m_Reader.readNext();
    skipWhitespaces();
    QString currEx;
    QString currTr;

    while (!m_Reader.atEnd() && !m_Reader.isEndElement()) {
        if (m_Reader.isStartElement()) {
            if (m_Reader.name() == "ex") {
                currEx = m_Reader.readElementText();
                m_Reader.readNext();
                skipWhitespaces();
            } else if (m_Reader.name() == "tr") {
                currTr = m_Reader.readElementText();
                m_Reader.readNext();
                skipWhitespaces();
            } else {
                skipUnknownElement();
            }
        }
    }

    m_Reader.readNext();
    card.addExample(currEx, currTr);
}

void XmlReader::skipUnknownElement()
{
    m_Reader.readNext();
    skipWhitespaces();
    while (!m_Reader.atEnd() && !m_Reader.isEndElement()) {
        if (m_Reader.isStartElement()) {
            skipUnknownElement();
        } else {
            m_Reader.readNext();
            skipWhitespaces();
        }
    }

    m_Reader.readNext();
}

/* FIXME: without this function program goes into an infinite loop
 if there are a lot of spaces in the file... */
void XmlReader::skipWhitespaces() {
    if (m_Reader.isWhitespace()) {
        m_Reader.readNext();
    }
}

QString XmlReader::getErrorMessage() {
    return m_ErrorMessage;
}
