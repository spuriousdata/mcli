#include "mci.h"
#include "connect_dialog.h"
#include "ConfigDialog.h"
#include "ui_mci.h"

#include <QtGui>

McI::McI(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::McIClass)
{
    ui->setupUi(this);
    connect(ui->action_quick_connect, SIGNAL(triggered()), this, SLOT(openConnectDialog()));
    connect(ui->add_button, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(ui->delete_button, SIGNAL(clicked()), this, SLOT(deleteClicked()));
    connect(ui->get_button, SIGNAL(clicked()), this, SLOT(getClicked()));
    connect(ui->flushall_button, SIGNAL(clicked()), this, SLOT(flushallClicked()));
    connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(openConfigDialog()));
}

void McI::openConfigDialog()
{
    if (!config_dialog) {
        config_dialog = new ConfigDialog(this);
    }
    config_dialog->show();
    config_dialog->raise();
    config_dialog->activateWindow();
}

void McI::openConnectDialog()
{
    if (!connect_dialog) {
        connect_dialog = new ConnectDialog(this);
    }
    connect_dialog->show();
    connect_dialog->raise();
    connect_dialog->activateWindow();

//    connect((MemcacheClient*)connect_dialog->mc, SIGNAL(hasNewStats()), this, SLOT(displayStats()));
}

void McI::displayStats()
{

}

void McI::addClicked()
{
    qDebug("addClicked() called");
}

void McI::getClicked()
{
    qDebug("getClicked() called");
}

void McI::deleteClicked()
{
    qDebug("deleteClicked() called");
}
void McI::flushallClicked()
{
    qDebug("flushallClicked() called");
}

McI::~McI()
{
    delete ui;
    delete connect_dialog;
}
