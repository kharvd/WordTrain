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

    mWriter.writeEmptyElement("learned");
    mWriter.writeAttribute("value", QString::number(it->isLearned()));

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
