#-------------------------------------------------
#
# Project created by QtCreator 2014-10-20T01:59:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = generatorGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    interlocutor.cpp

CONFIG += c++11

HEADERS  += mainwindow.h \
    interlocutor.h

#LIBS += -L"D:\Brovtsev\generatorGUI\Visa32.dll"
#LIBS += -L"D:\Brovtsev\generatorGUI\tkVisa32.dll"

LIBS += -L$$PWD/dll/Visa32.dll
LIBS += -L$$PWD/dll/tkVisa32.dll

#INCLUDEPATH += D://Brovtsev//generatorGUI//
#LIBS += -L"D://Brovtsev//generatorGUI//Visa32.lib"-lVisa32.lib
#LIBS += -L"D://Brovtsev//generatorGUI//tkVisa32.lib"-ltkVisa32.lib

#LIBS += -L"D://Brovtsev//generatorGUI//Visa32.dll"-lVisa32.dll
#LIBS += -L"D://Brovtsev//generatorGUI//tkVisa32.dll"-ltkVisa32.dll
