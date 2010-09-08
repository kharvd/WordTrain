#include <QtGui/QApplication>
#include <QtGui>
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



    MainWindow w;
    w.show();

    return a.exec();
}
