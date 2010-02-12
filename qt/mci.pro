# -------------------------------------------------
# Project created by QtCreator 2009-06-02T09:47:17
# -------------------------------------------------
QT += network
TARGET = McI
TEMPLATE = app
SOURCES += main.cc \
    mci.cc \
    HostEntry.cc \
    singlesocket.cc \
    MemcacheClient.cc \
    ConnectDialog.cc \
    ConfigDialog.cc \
    TreeNode.cc \
    StatsModel.cc \
    TreeModel.cc \
    StatData.cc \
    KeyValuePrompt.cc \
    AppController.cc \
    UIController.cc \
    DialogState.cc \
    BusyIndicator.cc \
    GetModel.cc \
    GetData.cc \
    GetItem.cc
HEADERS += mci.h \
    HostEntry.h \
    singlesocket.h \
    MemcacheClient.h \
    ConnectDialog.h \
    ConfigDialog.h \
    TreeNode.h \
    StatsModel.h \
    TreeModel.h \
    StatData.h \
    KeyValuePrompt.h \
    AppController.h \
    UIController.h \
    DialogState.h \
    macros.h \
    BusyIndicator.h \
    GetModel.h \
    GetData.h \
    GetItem.h
FORMS += mci.ui \
    ConnectDialog.ui \
    config.ui \
    KeyValuePrompt.ui \
    BusyIndicator.ui
CONFIG += embed_manifest_exe \
    embed_manifest_dll
#RC_FILE += mci.rc
