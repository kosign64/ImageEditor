#-------------------------------------------------
#
# Project created by QtCreator 2013-03-06T22:25:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Image_Editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mylabel.cpp

HEADERS  += mainwindow.h \
    mylabel.h

FORMS    += mainwindow.ui

RC_FILE = icon.rc

RESOURCES += \
    Resources.qrc
