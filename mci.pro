# -------------------------------------------------
# Project created by QtCreator 2009-06-02T09:47:17
# -------------------------------------------------
QT += network
TARGET = mci
TEMPLATE = app
SOURCES += main.cc \
    mci.cc \
    connect_dialog.cc \
    hostentry.cc \
    singlesocket.cc \
    memcacheclient.cc \
    ConfigDialog.cc \
    TreeNode.cc \
    StatsModel.cc \
    TreeModel.cc \
    StatData.cc
HEADERS += mci.h \
    connect_dialog.h \
    hostentry.h \
    singlesocket.h \
    memcacheclient.h \
    ConfigDialog.h \
    TreeNode.h \
    StatsModel.h \
    TreeModel.h \
    StatData.h
FORMS += mci.ui \
    connect_dialog.ui \
    config.ui
