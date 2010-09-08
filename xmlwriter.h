#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QXmlStreamWriter>
#include <QtGui>
#include <QVector>
#include <QPair>
#include "wordscard.h"

class XmlWriter
{
public:
    XmlWriter(const WordsSet *cards);
    bool writeFile(const QString & fileName);
    QString getErrorMessage();
private:
    void writeCard(WordsSet::const_iterator it);
    void writeExample(Examples::const_iterator it);
    QXmlStreamWriter mWriter;
    const WordsSet *mCards;
    QString mErrorMessage;
};

#endif // XMLWRITER_H
