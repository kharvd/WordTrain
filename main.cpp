#include <QtGui>
#include <QTranslator>
#include "mainwindow.h"
#include "exampleswidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Demihalf");
    QCoreApplication::setApplicationName("WordTrain");

    /* Для поддержки русских символов */
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString(":/wordtrain_") + locale);
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
