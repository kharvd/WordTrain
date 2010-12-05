/******************************************************************************
** WordTrain 0.9.1 -- Foreign words trainer
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
    mCards = cards;
}

bool XmlReader::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        mErrorMessage = QString("Error: Cannot read file ")
                        + qPrintable(fileName)
                        + ": " + qPrintable(file.errorString());
        return false;
    }

    mReader.setDevice(&file);

    mReader.readNext();
    skipWhitespaces();

    while (!mReader.atEnd()) {
        if (mReader.isStartElement()) {
            if (mReader.name() == "wordsset") {
                readWordsSet();
            } else {
                mReader.raiseError(QObject::tr("Not a words set file"));
            }
        } else {
            mReader.readNext();
            skipWhitespaces();
        }
    }

    file.close();
    if (mReader.hasError()) {
        mErrorMessage = QString("Error: Failed to parse file ")
                        + qPrintable(fileName) + ": "
                        + qPrintable(mReader.errorString());
        return false;
    } else if (file.error() != QFile::NoError) {
        mErrorMessage = QString("Error: Cannot read file ")
                        + qPrintable(fileName) + ": "
                        + qPrintable(file.errorString());
        return false;
    }
    return true;
}

void XmlReader::readWordsSet()
{
    mReader.readNext();
    skipWhitespaces();

    while (!mReader.atEnd() && !mReader.isEndElement()) {
        if (mReader.isStartElement()) {
            if (mReader.name() == "wordcard") {
                WordCard curr;
                mCards->push_back(curr);
                readWordCard();
            } else {
                skipUnknownElement();
            }
        } else {
            mReader.readNext();
            skipWhitespaces();
        }
    }

    mReader.readNext();
}

void XmlReader::readWordCard()
{
    WordCard & card = mCards->last();
    mReader.readNext();
    skipWhitespaces();

    while (!mReader.atEnd() && !(mReader.isEndElement()
        && mReader.name() == "wordcard"))
    {
        if (mReader.isStartElement()) {
            if (mReader.name() == "word") {
                card.setWord(mReader.attributes().value("value").toString());
                mReader.readNext();
                skipWhitespaces();
            } else if (mReader.name() == "transcription") {
                card.setTranscription(
                        mReader.attributes().value("value").toString());
                mReader.readNext();
                skipWhitespaces();
            } else if (mReader.name() == "translation") {
                card.setTranslation(
                        mReader.attributes().value("value").toString());
                mReader.readNext();
                skipWhitespaces();
            } else if (mReader.name() == "plural") {
                card.setPlural(
                        mReader.attributes().value("value").toString());
                mReader.readNext();
                skipWhitespaces();
            } else if (mReader.name() == "gender") {
                card.setGender(
                        mReader.attributes().value("value").toString().toInt());
                mReader.readNext();
                skipWhitespaces();
            } else if (mReader.name() == "category") {
                card.setCategory(
                        mReader.attributes().value("value").toString().toInt());
                mReader.readNext();
                skipWhitespaces();
            } else if (mReader.name() == "answers") {
                card.setCorrectAnswers
(
                        mReader.attributes().value("value").toString().toInt());
                mReader.readNext();
                skipWhitespaces();
            } else if (mReader.name() == "example") {
                readExample();
            } else {
                skipUnknownElement();
            }
        } else {
            mReader.readNext();
            skipWhitespaces();
        }
    }

    mReader.readNext();
}

void XmlReader::readExample() {
    WordCard & card = mCards->last();

    mReader.readNext();
    skipWhitespaces();
    QString currEx;
    QString currTr;

    while (!mReader.atEnd() && !mReader.isEndElement()) {
        if (mReader.isStartElement()) {
            if (mReader.name() == "ex") {
                currEx = mReader.readElementText();
                mReader.readNext();
                skipWhitespaces();
            } else if (mReader.name() == "tr") {
                currTr = mReader.readElementText();
                mReader.readNext();
                skipWhitespaces();
            } else {
                skipUnknownElement();
            }
        }
    }

    mReader.readNext();
    card.addExample(currEx, currTr);
}

void XmlReader::skipUnknownElement()
{
    mReader.readNext();
    skipWhitespaces();
    while (!mReader.atEnd() && !mReader.isEndElement()) {
        if (mReader.isStartElement()) {
            skipUnknownElement();
        } else {
            mReader.readNext();
            skipWhitespaces();
        }
    }

    mReader.readNext();
}

/* FIXME: without this function program goes into an infinite loop
 if there are a lot of spaces in the file... */
void XmlReader::skipWhitespaces() {
    if (mReader.isWhitespace()) {
        mReader.readNext();
    }
}

QString XmlReader::getErrorMessage() {
    return mErrorMessage;
}
