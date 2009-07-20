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

ConfigDialog::~ConfigDialog()
{
	delete m_ui->socks_port->validator();
	delete m_ui;
}

