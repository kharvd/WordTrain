/******************************************************************************
** WordTrain 0.8.5 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov <kharvd@gmail.com>
**
** This file is part of WordTrain.
**
** $QT_BEGIN_LICENSE:GPL$
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QT_END_LICENSE$
**
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#include <QtGui>
#include <QTranslator>
#include "mainwindow.h"
#include "exampleswidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Demihalf");
    QCoreApplication::setApplicationName("WordTrain");
    QCoreApplication::setApplicationVersion("0.8.5");

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

    a.setWindowIcon(QIcon(":/icons/icon.png"));

    MainWindow w;
    w.show();

    return a.exec();
}
