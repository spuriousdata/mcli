#include "connect_dialog.h"
#include "ui_connect_dialog.h"
#include "memcacheclient.h"
#include <QLabel>
#include <QSpacerItem>
#include <QRegExp>
#include <QHBoxLayout>
#include <QLineEdit>

ConnectDialog::ConnectDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::ConnectDialog),
	mc(new MemcacheClient())
{
	m_ui->setupUi(this);
	connect(m_ui->addserver_button, SIGNAL(clicked()), this, SLOT(addServerClicked()));
	connect(m_ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(memcache_connect()));
	m_ui->scrolling_contents->setLayout(new QVBoxLayout);
	m_ui->scrolling_contents->layout()->setContentsMargins(0, 0, 0, 0);
	m_ui->scrolling_contents->layout()->setSizeConstraint(QLayout::SetMinimumSize);

	connect(mc, SIGNAL(hasNewStats()), parent, SLOT(displayStats()));
}

void ConnectDialog::addServerClicked()
{
	this->addHostEntry();
}

void ConnectDialog::memcache_connect()
{
	mc->mc_connect(&servers);
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

void ConnectDialog::addHostEntry()
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
