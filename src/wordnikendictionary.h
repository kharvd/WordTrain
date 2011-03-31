#ifndef WORDNIKENDICTIONARY_H
#define WORDNIKENDICTIONARY_H

#include "abstractdictionary.h"

#include <QtCore>

class QNetworkAccessManager;
class QNetworkReply;

class WordnikEnDictionary : public AbstractDictionary
{
    Q_OBJECT

public:
    explicit WordnikEnDictionary(QObject *parent = 0);
    virtual ~WordnikEnDictionary() {}

    // -1 means all
    virtual QStringList getDefinition(const QString &word, int count = -1);
    virtual QStringList getExamples(const QString &word, int count = -1);
    virtual QString getPronunciation(const QString &word);

private:
    static const QString m_ApiKey;
    static const QString m_ApiBaseUrl;

    QStringList m_Definition;
    QStringList m_Examples;
    QString m_Pronunciation;
};

#endif // WORDNIKENDICTIONARY_H
