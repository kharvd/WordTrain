#include "wordnikendictionary.h"

#include <QtCore>
#include <QtXml>
#include <QtNetwork>

const QString WordnikEnDictionary::m_ApiKey
        = "bd180a9c72e88f7927219061a2d08b0c34ad9b4f7fc7b14bc";
const QString WordnikEnDictionary::m_ApiBaseUrl = "http://api.wordnik.com/v4/";

WordnikEnDictionary::WordnikEnDictionary(QObject *parent) :
    AbstractDictionary(parent)
{
}

QStringList WordnikEnDictionary::getDefinition(const QString &word, int count)
{
    return QStringList();
}

QStringList WordnikEnDictionary::getExamples(const QString &word, int count)
{
    QNetworkAccessManager *exMan = new QNetworkAccessManager();

    QString request = m_ApiBaseUrl + "word.xml/%1/examples?api_key=%2";
    request = request.arg(word).arg(m_ApiKey);
    request += "&useCanonical=1&includeDuplicates=0";

    if (count > 0)
        request += QString("&limit=%1").arg(count);

    QNetworkReply *reply = exMan->get(QNetworkRequest(QUrl(request)));

    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }

    m_IsError = reply->error() != QNetworkReply::NoError;
    m_ErrorMessage = reply->errorString();

    QStringList list;

    if (!m_IsError) {
        QXmlStreamReader reader;
        reader.setDevice(reply);

        while (!reader.atEnd() && (reader.name() != "raw")) {
            reader.readNext();
            if (reader.name() == "text") {
                list << reader.readElementText();
            }
        }
    }

    reply->deleteLater();
    return list;
}

QString WordnikEnDictionary::getPronunciation(const QString &word)
{
    QNetworkAccessManager *pronunMan = new QNetworkAccessManager();

    QString request = m_ApiBaseUrl + "word.xml/%1/pronunciations?api_key=%2";
    request = request.arg(word).arg(m_ApiKey);
    request += "&useCanonical=1";

    QNetworkReply *reply = pronunMan->get(QNetworkRequest(QUrl(request)));

    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }

    m_IsError = reply->error() != QNetworkReply::NoError;
    m_ErrorMessage = reply->errorString();

    QString pron;

    if (!m_IsError) {
        QXmlStreamReader reader;
        reader.setDevice(reply);

        while (!reader.atEnd() && (reader.name() != "raw")) {
            reader.readNext();
            if (reader.name() == "raw") {
                pron = reader.readElementText();
            }
        }

        // Chop parenthesis
        pron.chop(1);
        pron.remove(0, 1);
    }

    reply->deleteLater();
    return pron;
}
