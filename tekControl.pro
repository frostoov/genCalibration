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
	ui/teksettings.cpp \
    muons/bitslib.cpp \
    muons/create_dir.cpp \
    muons/flash_adc.cpp \
    muons/getsumpls.cpp \
    muons/global_vars.cpp \
    muons/mainblck.cpp \
    muons/master.cpp \
    muons/muons_2012.cpp \
    muons/readconf.cpp \
    muons/savedata.cpp \
    muons/sock.cpp \
    muons/stoprun.cpp \
    muons/thresholds.cpp \
    muons/vme.cpp \
    muons/vme-lib.cpp \
    tekVisa/chipmodule.cpp


HEADERS  += \
	observer/observer.h \
	tekVisa/tekmodule.h \
	tekVisa/tektypes.h \
	ui/mainwindow.h \
	ui/tekoutput.h \
	ui/teksettings.h \
	visa/visa.h \
	visa/visatype.h \
    ui/types.hpp \
    muons/bitslib.h \
    muons/common.h \
    muons/create_dir.h \
    muons/defines.h \
    muons/flash_adc.h \
    muons/getsumpls.h \
    muons/globals_clT.h \
    muons/mainblck.h \
    muons/master.h \
    muons/muons_2012.h \
    muons/newsock.h \
    muons/readconf.h \
    muons/savedata.h \
    muons/sock.h \
    muons/stoprun.h \
    muons/thresholds.h \
    muons/vme.hpp \
    muons/vme-lib.h \
    tekVisa/chipmodule.h


LIBS        +=	-L$$PWD/lib/ -lvisa
INCLUDEPATH +=	  $$PWD/lib/

release {
	QMAKE_CXXFLAGS_RELEASE	-= -O1
	QMAKE_CXXFLAGS_RELEASE	-= -O2
	QMAKE_CXXFLAGS_RELEASE	+= -O3
}
