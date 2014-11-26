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
	observer/observer.cpp \
	tekVisa/tekmodule.cpp \
	ui/main.cpp \
	ui/mainwindow.cpp \
	ui/tekoutput.cpp \
	ui/teksettings.cpp

HEADERS  += \
	observer/observer.h \
	tekVisa/tekmodule.h \
	tekVisa/tektypes.h \
	ui/mainwindow.h \
	ui/tekoutput.h \
	ui/teksettings.h \
	visa/visa.h \
	visa/visatype.h \
    ui/types.hpp

LIBS        +=	-L$$PWD/lib/ -lvisa
INCLUDEPATH +=	  $$PWD/lib/

release {
	QMAKE_CXXFLAGS_RELEASE	-= -O1
	QMAKE_CXXFLAGS_RELEASE	-= -O2
	QMAKE_CXXFLAGS_RELEASE	+= -O3
}
