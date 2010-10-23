#include <QtGui>
#include <QTranslator>
#include "mainwindow.h"
#include "exampleswidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Demihalf");
    QCoreApplication::setApplicationName("WordTrain");

    // Language set
    QSettings sets;
    QString lang = sets.value("lang", "auto").toString();

    if (lang == "auto") {
        lang = QLocale::system().name();
    }

    QTranslator translatorQt;
    translatorQt.load(QString(":/translations/qt_") + lang);
    a.installTranslator(&translatorQt);

    QTranslator translator;
    translator.load(QString(":/translations/wordtrain_") + lang);
    a.installTranslator(&translator);

    //====================

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    MainWindow w;
    w.show();

    return a.exec();
}
