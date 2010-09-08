#ifndef XMLSTREAMREADER_H
#define XMLSTREAMREADER_H

#include <QtGui>
#include <QList>
#include <QString>
#include <QXmlStreamReader>
#include "wordscard.h"

/* XML reader for words training files (wsf) */
class XmlReader
{
public:
    XmlReader(WordsSet *cards);

    bool readFile(const QString &fileName);
    QString getErrorMessage();
private:
    void readWordsSet();
    void readWordCard();
    void readExample();
    void skipUnknownElement();
    void skipWhitespaces();

    WordsSet *mCards;
    QXmlStreamReader mReader;
    QString mErrorMessage;
};

#endif // XMLSTREAMREADER_H
