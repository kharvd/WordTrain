/******************************************************************************
** WordTrain 0.8.4 -- Foreign words trainer
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

XmlWriter::XmlWriter(const WordsSet *cards)
{
    mCards = cards;
    mWriter.setAutoFormatting(true);
}

bool XmlWriter::writeFile(const QString & fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        mErrorMessage = QString("Error: Cannot open file ")
                        + qPrintable(fileName)
                        + ": " + qPrintable(file.errorString());
        return false;
    }

    mWriter.setDevice(&file);

    mWriter.writeStartDocument();
    mWriter.writeStartElement("wordsset");

    WordsSet::const_iterator it = mCards->begin();
    for ( ; it != mCards->end(); it++) {
        writeCard(it);
    }

    mWriter.writeEndElement();

    return true;
}

void XmlWriter::writeCard(WordsSet::const_iterator it) {
    mWriter.writeStartElement("wordcard");

    mWriter.writeEmptyElement("word");
    mWriter.writeAttribute("value", it->word());

    mWriter.writeEmptyElement("transcription");
    mWriter.writeAttribute("value", it->transcription());

    mWriter.writeEmptyElement("translation");
    mWriter.writeAttribute("value", it->translation());

    mWriter.writeEmptyElement("plural");
    mWriter.writeAttribute("value", it->plural());

    mWriter.writeEmptyElement("gender");
    mWriter.writeAttribute("value", QString::number(it->gender()));

    mWriter.writeEmptyElement("category");
    mWriter.writeAttribute("value", QString::number(it->category()));

    mWriter.writeEmptyElement("answers");
    mWriter.writeAttribute("value", QString::number(it->numCorrectAnswers()));

    Examples::const_iterator exIt = it->examples().begin();
    for ( ; exIt != it->examples().end(); exIt++) {
        writeExample(exIt);
    }

    mWriter.writeEndElement();
}

void XmlWriter::writeExample(Examples::const_iterator it) {
    mWriter.writeStartElement("example");
    mWriter.writeTextElement("ex", it->first);
    mWriter.writeTextElement("tr", it->second);
    mWriter.writeEndElement();
}

QString XmlWriter::getErrorMessage() {
    return mErrorMessage;
}
