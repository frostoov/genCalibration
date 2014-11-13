#-------------------------------------------------
#
# Project created by QtCreator 2014-10-20T01:59:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET		= tekControl
TEMPLATE	= app
CONFIG		+= c++11

SOURCES += \
	main.cpp\
	mainwindow.cpp \
    observer.cpp \
    teksettings.cpp \
    tekmodule.cpp \
    tekoutput.cpp

HEADERS  += \
	mainwindow.h \
    visa.h \
    visatype.h \
    observer.h \
    teksettings.h \
    tekmodule.h \
    tektypes.h \
    tekoutput.h

linux{
	LIBS        +=	-L$$PWD/lib/ -lvisa
	INCLUDEPATH +=	  $$PWD/lib/
}
windows{
	LIBS += -L$$PWD/dll/Visa32.dll
	LIBS += -L$$PWD/dll/tkVisa32.dll
}

release {
	QMAKE_CXXFLAGS_RELEASE	-= -O1
	QMAKE_CXXFLAGS_RELEASE	-= -O2
	QMAKE_CXXFLAGS_RELEASE	*= -O3
}
