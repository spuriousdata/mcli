#include "ConfigDialog.h"
#include "ui_config.h"
#include "hostentry.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    connect(ui->socks_proto, SIGNAL(valueChanged(int)), this, SLOT(toggle_unpw(int)));
    ui->socks_port->setValidator(
        new QRegExpValidator(QRegExp("[1-9][0-9]{0,4}"), this)
    );
}

/**
 * Only enable username and password fields if SOCKS proto is 5
 * SOCKS4 doesn't support authentication
 */
void ConfigDialog::toggle_unpw(int val)
{
    bool enable;

    enable = ((val == 5) ? true : false);

    ui->socks_username->setEnabled(enable);
    ui->socks_username_label->setEnabled(enable);
    ui->socks_password->setEnabled(enable);
    ui->socks_password_label->setEnabled(enable);
}

ConfigDialog::~ConfigDialog()
{
    delete ui->socks_port->validator();
    delete ui;
}

