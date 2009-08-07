#include "ConfigDialog.h"
#include "ui_config.h"
#include "hostentry.h"
#include <QtGui>

ConfigDialog::ConfigDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::ConfigDialog)
{
	m_ui->setupUi(this);
	connect(m_ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SIGNAL(configDone()));
	m_ui->socks_port->setValidator(
		new QRegExpValidator(QRegExp("[1-9][0-9]{0,4}"), this)
	);
}

void ConfigDialog::setUseSocks(bool f)
{
	m_ui->use_socks->setChecked(f);
}

void ConfigDialog::setSocksDns(bool f)
{
	m_ui->socks_dns->setChecked(f);
}

void ConfigDialog::setSocksHost(QString host)
{
	m_ui->socks_hostname->setText(host);
}

void ConfigDialog::setSocksPort(QString port)
{
	m_ui->socks_port->setText(port);
}

void ConfigDialog::setSocksUser(QString user)
{
	m_ui->socks_username->setText(user);
}

void ConfigDialog::setSocksPass(QString pass)
{
	m_ui->socks_password->setText(pass);
}

ConfigDialog::~ConfigDialog()
{
	delete m_ui->socks_port->validator();
	delete m_ui;
}

