# -------------------------------------------------
# Project created by QtCreator 2009-06-02T09:47:17
# -------------------------------------------------
QT += network
TARGET = mci
TEMPLATE = app
SOURCES += main.cpp \
    mci.cpp \
    connect_dialog.cpp \
    hostentry.cpp \
    singlesocket.cpp \
    memcacheclient.cpp
HEADERS += mci.h \
    connect_dialog.h \
    hostentry.h \
    singlesocket.h \
    memcacheclient.h
FORMS += mci.ui \
    connect_dialog.ui \
	config.ui
