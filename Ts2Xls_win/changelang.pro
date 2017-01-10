#-------------------------------------------------
#
# Project created by QtCreator 2012-08-16T17:33:16
#
#-------------------------------------------------

QT          += core gui
QT          += xml
CONFIG  += qaxcontainer
CONFIG  += console

TARGET = changelang
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    xmlserver.cpp \
    ExcelBase.cpp

HEADERS  += dialog.h \
    xmlserver.h \
    ExcelBase.h

FORMS    += dialog.ui
