#-------------------------------------------------
#
# Project created by QtCreator 2014-10-20T01:59:25
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

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
    tekVisa/chipmodule.cpp \
    chip/muons_2012.cpp \
    chip/vme.cpp \
    chip/create_dir.c \
    chip/flash_adc.c \
    chip/getsumpls.c \
    chip/mainblck.c \
    chip/master.c \
    chip/readconf.c \
    chip/savedata.c \
    chip/stoprun.c \
    chip/thresholds.c \
    chip/vme-lib.c \
    chip/blocks.cpp \
    ui/plots.cpp \
    tekVisa/processing.cpp



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
    tekVisa/chipmodule.h \
    chip/bitslib.h \
    chip/common.h \
    chip/defines.h \
    chip/flash_adc.h \
    chip/globals_clT.h \
    chip/master.h \
    chip/sock.h \
    chip/vme.h \
    chip/blocks.h \
    ui/plots.h  \
    qcustomplot.h \
    tekVisa/processing.h



#LIBS        +=  -L$$PWD/lib/ -lqcustomplot
#LIBS        +=	-L$$PWD/lib/ -lvisa
LIBS         += -lvisa
LIBS         += -lqcustomplot
INCLUDEPATH +=	  $$PWD/lib/

INCLUDEPATH +=  $$PWD/qcustomplot/

release {
	QMAKE_CXXFLAGS_RELEASE	-= -O1
	QMAKE_CXXFLAGS_RELEASE	-= -O2
	QMAKE_CXXFLAGS_RELEASE	+= -O3
}
