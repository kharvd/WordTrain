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
                        + qPrintable(fileName)
                        + qPrintable(mReader.errorString());
        return false;
    } else if (file.error() != QFile::NoError) {
        mErrorMessage = QString("Error: Cannot read file ")
                        + qPrintable(fileName)
                        + ": " + qPrintable(file.errorString());
        return false;
    }
    return true;
}

void XmlReader::readWordsSet()
{
    mReader.readNext();
    skipWhitespaces();

    while (!mReader.atEnd()) {
        if (mReader.isEndElement()) {
            mReader.readNext();
            break;
        }

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
}

void XmlReader::readWordCard()
{
    WordCard & card = mCards->last();
    mReader.readNext();
    skipWhitespaces();

    while (!mReader.atEnd()) {
        if (mReader.isEndElement() && mReader.name() == "wordcard") {
            mReader.readNext();
            break;
        }

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
                card.setNumCorrectAnswers(
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
}

void XmlReader::readExample() {
    WordCard & card = mCards->last();

    mReader.readNext();
    skipWhitespaces();
    QString currEx;
    QString currTr;

    while (!mReader.atEnd()) {
        if (mReader.isEndElement()) {
            mReader.readNext();
            break;
        }
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

    card.addExample(currEx, currTr);
}

void XmlReader::skipUnknownElement()
{
    mReader.readNext();
    skipWhitespaces();
    while (!mReader.atEnd()) {
        if (mReader.isEndElement()) {
            mReader.readNext();
            break;
        }

        if (mReader.isStartElement()) {
            skipUnknownElement();
        } else {
            mReader.readNext();
            skipWhitespaces();
        }
    }
}

/* FIXME: without this function program goes in infinite loop
 if there are a lot of spaces in the file... */
void XmlReader::skipWhitespaces() {
    if (mReader.isWhitespace()) {
        mReader.readNext();
    }
}

QString XmlReader::getErrorMessage() {
    return mErrorMessage;
}
