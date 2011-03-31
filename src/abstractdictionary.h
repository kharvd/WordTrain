#ifndef ABSTRACTONLINEDICTIONARY_H
#define ABSTRACTONLINEDICTIONARY_H

#include <QObject>
#include "wordcard.h"

class AbstractDictionary : public QObject
{
    Q_OBJECT

public:
    explicit AbstractDictionary(QObject *parent = 0);
    virtual ~AbstractDictionary() {}

    // -1 means all
    virtual QStringList getDefinition(const QString &word, int count = -1) = 0;
    virtual QStringList getExamples(const QString &word, int count = -1) = 0;
    virtual QString getPronunciation(const QString &word) = 0;
    virtual QString errorMessage() { return m_ErrorMessage; }
    virtual bool isError() { return m_IsError; }

protected:
    bool m_IsError;
    QString m_ErrorMessage;
};

#endif // ABSTRACTONLINEDICTIONARY_H
