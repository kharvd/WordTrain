# -------------------------------------------------
# Project created by QtCreator 2010-08-05T08:48:00
# -------------------------------------------------
QT += core \
    gui \
    xml \
    testlib
CONFIG += warn_on \
    qt
TARGET = ../bin/wordtrain
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    wordcard.cpp \
    xmlreader.cpp \
    xmlwriter.cpp \
    neweditcarddialog.cpp \
    exampleswidget.cpp \
    startquizdialog.cpp \
    answerwidget.cpp \
    viewcarddialog.cpp \
    cardwidget.cpp \
    lineanswerwidget.cpp \
    quizdialog.cpp \
    multianswerwidget.cpp \
    settingsdialog.cpp \
    aboutdialog.cpp \
    gettingstartedwidget.cpp \
    quiz.cpp \
    quizquestion.cpp \
    wordfilter.cpp \
    searchwordfilter.cpp \
    tagwordfilter.cpp \
    quizwordschooserdialog.cpp \
    tagsscrollarea.cpp \
    quizwordschoosertable.cpp \
    learnedwordfilter.cpp
HEADERS += mainwindow.h \
    wordcard.h \
    xmlreader.h \
    xmlwriter.h \
    neweditcarddialog.h \
    exampleswidget.h \
    startquizdialog.h \
    answerwidget.h \
    viewcarddialog.h \
    cardwidget.h \
    lineanswerwidget.h \
    quizdialog.h \
    multianswerwidget.h \
    utilities.h \
    settingsdialog.h \
    aboutdialog.h \
    gettingstartedwidget.h \
    quiz.h \
    quizquestion.h \
    wordfilter.h \
    searchwordfilter.h \
    tagwordfilter.h \
    quizwordschooserdialog.h \
    tagsscrollarea.h \
    quizwordschoosertable.h \
    learnedwordfilter.h
RESOURCES += resources.qrc
TRANSLATIONS = wordtrain_ru.ts #wordtrain_de.ts

RC_FILE = wordtrain.rc
icon.files += wordtrain.png
desc.files += ../WordTrain.desktop
target.path = /usr/local/bin/
icon.path = /usr/local/share/pixmaps/
desc.path = /usr/local/share/applications/
INSTALLS += target desc icon
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
RCC_DIR = ../tmp
