#include "AppController.h"
#include "UIController.h"
#include "MemcacheClient.h"
#include "ConnectDialog.h"
#include "ConfigDialog.h"
#include "KeyValuePrompt.h"
#include "ui_config.h"
#include "HostEntry.h"
#include "StatData.h"
#include "GetData.h"
#include <QSettings>
#include <QVector>
#include <QNetworkProxy>

AppController::AppController()
{
	ui_controller = new UIController(this);
	memcache = new MemcacheClient(this);

	connect(ui_controller, SIGNAL(doConnect()), this, SLOT(mcConnect()));
	connect(ui_controller, SIGNAL(doConfigDone()), this, SLOT(configureFromDialog()));
	connect(ui_controller,SIGNAL(doAdd()), this, SLOT(addItem()));
	connect(ui_controller, SIGNAL(doGet()), this, SLOT(getItem()));
	connect(ui_controller, SIGNAL(doDelete()), this, SLOT(deleteItem()));
	connect(ui_controller, SIGNAL(doFlushAll()), memcache, SLOT(flushAll()));
	connect(ui_controller, SIGNAL(doGetStats()), memcache, SLOT(getStats()));
	connect(memcache, SIGNAL(hasNewStats(QVector<StatData *>&)), ui_controller, SIGNAL(hasNewStats(QVector<StatData *>&)));
	connect(memcache, SIGNAL(hasNewGet(QVector<GetData *>&)), ui_controller, SIGNAL(hasNewGet(QVector<GetData *>&)));

	configure();
}

void AppController::setBusy(bool isBusy)
{
	ui_controller->setBusy(isBusy);
}

void AppController::alert(QString title, QString body)
{
	ui_controller->alert(title, body);
}

void AppController::configure()
{
	QSettings settings(settingsOrg(), settingsName());

	settings.beginGroup("SocksProxy");
	if (settings.value("use").toBool()) {
		if (settings.value("user").toString().length() > 0) {
			QNetworkProxy::setApplicationProxy(
					QNetworkProxy(
							QNetworkProxy::Socks5Proxy,
							settings.value("host").toString(),
							settings.value("port").toInt(),
							settings.value("user").toString(),
							settings.value("pass").toString()
					)
			);
		} else {
			QNetworkProxy::setApplicationProxy(
				QNetworkProxy(
					QNetworkProxy::Socks5Proxy,
					settings.value("host").toString(),
					settings.value("port").toInt()
					)
			);
		}
	}
}

void AppController::configureFromDialog()
{
	QSettings settings(settingsOrg(), settingsName());
	QString proxyhost, proxyport, proxyuser, proxypass;
	bool use_socks;
	bool socks_dns;

	use_socks = ui_controller->getConfigDialog()->ui()->use_socks->isChecked();

	if (use_socks) {
		proxyhost = ui_controller->getConfigDialog()->ui()->socks_hostname->text();
		proxyport = ui_controller->getConfigDialog()->ui()->socks_port->text();
		proxyuser = ui_controller->getConfigDialog()->ui()->socks_username->text();
		proxypass = ui_controller->getConfigDialog()->ui()->socks_password->text();

		socks_dns = ui_controller->getConfigDialog()->ui()->socks_dns->isChecked();

		settings.beginGroup("SocksProxy");
		settings.setValue("use", true);
		settings.setValue("host", proxyhost);
		settings.setValue("port", proxyport);
		settings.setValue("user", proxyuser);
		settings.setValue("pass", proxypass);
		settings.setValue("dns", socks_dns);
		settings.endGroup();

		if (proxyuser.length() > 0) {
			QNetworkProxy::setApplicationProxy(
				QNetworkProxy(
						QNetworkProxy::Socks5Proxy,
					proxyhost,
					proxyport.toInt(),
					proxyuser,
					proxypass
				)
			);
		} else {
			QNetworkProxy::setApplicationProxy(
					QNetworkProxy(
						QNetworkProxy::Socks5Proxy,
						proxyhost,
						proxyport.toInt()
					)
			);
		}
	} else {
		settings.beginGroup("SocksProxy");
		settings.setValue("use", false);
		settings.endGroup();

		QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::NoProxy));
	}

	// reconnect
	if (memcache->isConnected()) mcConnect();
}

void AppController::mcConnect()
{
	QSettings settings(settingsOrg(), settingsName());
	QStringList servers;
	HostEntry *he;
	QVector<HostEntry *> &he_list = ui_controller->getConnectDialog()->servers;

	foreach (he, he_list) {
		servers << he->host->text().append(":").append(he->port->text());
	}

	settings.beginGroup("Connection");
	settings.setValue("servers", servers);
	settings.endGroup();
	memcache->mc_connect(he_list);
}

void AppController::addItem()
{
	memcache->addItem(ui_controller->getKeyValuePrompt()->key(), ui_controller->getKeyValuePrompt()->value());
}

void AppController::getItem()
{
	memcache->getItem(ui_controller->getKeyValuePrompt()->key());
}

void AppController::run()
{
	ui_controller->showMainWindow();
}

AppController::~AppController()
{
	delete ui_controller;
	delete memcache;
}
