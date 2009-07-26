#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"
#include "HostEntry.h"
#include <QLabel>
#include <QSpacerItem>
#include <QRegExp>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QStringList>

ConnectDialog::ConnectDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::ConnectDialog)
{
	m_ui->setupUi(this);

	connect(m_ui->addserver_button, SIGNAL(clicked()), this, SLOT(addServerClicked()));
	connect(m_ui->removeserver_button, SIGNAL(clicked()), this, SLOT(removeServerClicked()));

	/* emit a connectRead() signal when the OK button is pushed */
	connect(m_ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SIGNAL(connectReady()));

	m_ui->scrolling_contents->setLayout(new QVBoxLayout);
	m_ui->scrolling_contents->layout()->setContentsMargins(0, 0, 0, 0);
	m_ui->scrolling_contents->layout()->setSizeConstraint(QLayout::SetMinimumSize);

}

void ConnectDialog::removeServerClicked()
{
	QLayoutItem *i;

	i = m_ui->scrolling_contents->layout()->takeAt(
		servers.size()-1
	);

	m_ui->scrolling_contents->layout()->removeItem(i);

	delete i;
	servers.pop_back();
}

void ConnectDialog::addServerClicked()
{
	this->addHostEntry();
}

void ConnectDialog::retab()
{
	QWidget *last;
	QWidget::setTabOrder(m_ui->buttonBox, m_ui->addserver_button);
	last = m_ui->addserver_button;
	for (QVector<HostEntry *>::iterator i = servers.begin(); i != servers.end(); i++) {
		QWidget::setTabOrder(last, (*i)->host);
		QWidget::setTabOrder((*i)->host, (*i)->port);
		last = (*i)->port;
	}
	QWidget::setTabOrder(last, m_ui->buttonBox);
}

void ConnectDialog::setSavedServers(QStringList savedServers)
{
	QString server;
	QStringList parts;

	foreach (server, savedServers) {
		parts = server.split(":");
		addHostEntry(parts[0], parts[1]);
	}
}

void ConnectDialog::addHostEntry(QString hostname, QString portnumber)
{
	QHBoxLayout *server_info_horiz_layout;
	QLabel *host_label;
	QSpacerItem *host_port_spacer;
	QLabel *port_label;
	QWidget *host_widget = new QWidget();
	HostEntry *host;

	m_ui->scrolling_contents->layout()->addWidget(host_widget);
	host = new HostEntry();
	server_info_horiz_layout = new QHBoxLayout(host_widget);
	server_info_horiz_layout->setObjectName(QString::fromUtf8("server_info_horiz_layout %1").arg(servers.count()));
	server_info_horiz_layout->setContentsMargins(0, 0, 0, 0);
	host_label = new QLabel(host_widget);
	host_label->setObjectName(QString::fromUtf8("host_label %1").arg(servers.count()));
	host_label->setText(QString::fromUtf8("Host %1: ").arg(servers.count()+1));

	server_info_horiz_layout->addWidget(host_label);

	host->host = new QLineEdit(host_widget);
	host->host->setObjectName(QString::fromUtf8("host_edit %1").arg(servers.count()));

	server_info_horiz_layout->addWidget(host->host);

	host_port_spacer = new QSpacerItem(28, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

	server_info_horiz_layout->addItem(host_port_spacer);

	port_label = new QLabel(host_widget);
	port_label->setObjectName(QString::fromUtf8("port_label %1").arg(servers.count()));
	port_label->setText(QString::fromUtf8("Port %1: ").arg(servers.count()+1));

	server_info_horiz_layout->addWidget(port_label);

	host->port = new QLineEdit(host_widget);
	host->port->setObjectName(QString::fromUtf8("port_edit %1").arg(servers.count()));
	host->port->setEnabled(true);
	QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy1.setHorizontalStretch(0);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(host->port->sizePolicy().hasHeightForWidth());
	host->port->setSizePolicy(sizePolicy1);
	host->port->setMaximumSize(QSize(70, 16777215));
	host->port->setMaxLength(5);

	host->port->setValidator(new QRegExpValidator(HostEntry::port_regex, this));

	server_info_horiz_layout->addWidget(host->port);
	host->host->setFocus();
	servers.append(host);

	if (hostname.length() > 0 && portnumber.length() > 0) {
		host->host->setText(hostname);
		host->port->setText(portnumber);
	}

	retab();
}

ConnectDialog::~ConnectDialog()
{
	HostEntry *h;

	foreach (h, servers) {
		delete h;
	}
	delete m_ui;
}

void ConnectDialog::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
