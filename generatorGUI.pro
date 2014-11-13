#-------------------------------------------------
#
# Project created by QtCreator 2014-10-20T01:59:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET		= generatorGUI
TEMPLATE	= app

SOURCES += \
	main.cpp\
	mainwindow.cpp \
    interlocutor.cpp

HEADERS  += \
	mainwindow.h \
    interlocutor.h \
    visa.h \
    visatype.h

linux{
	LIBS        +=	-L$$PWD/lib/ -lvisa
	INCLUDEPATH +=	  $$PWD/lib/
}
windows{
	LIBS += -L$$PWD/dll/Visa32.dll
	LIBS += -L$$PWD/dll/tkVisa32.dll
}
CONFIG		+= c++11
