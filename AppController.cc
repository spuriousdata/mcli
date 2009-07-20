#include "AppController.h"
#include "UIController.h"
#include "MemcacheClient.h"
#include "ConnectDialog.h"
#include "ConfigDialog.h"
#include "KeyValuePrompt.h"
#include "ui_config.h"
#include "HostEntry.h"
#include <QSettings>
#include <QVector>
#include <QNetworkProxy>

AppController::AppController()
{
	ui_controller = new UIController(this);
	memcache = new MemcacheClient(this);

	connect(ui_controller, SIGNAL(doConnect()), this, SLOT(mcConnect()));
	connect(ui_controller, SIGNAL(doConfigDone()), this, SLOT(configure()));
	connect(ui_controller,SIGNAL(doAdd()), this, SLOT(addItem()));
	connect(ui_controller, SIGNAL(doGet()), this, SLOT(getItem()));
	connect(ui_controller, SIGNAL(doDelete()), this, SLOT(deleteItem()));
	connect(ui_controller, SIGNAL(doFlushAll()), memcache, SLOT(flushAll()));
	connect(ui_controller, SIGNAL(doGetStats()), memcache, SLOT(getStats()));
	connect(memcache, SIGNAL(hasNewStats(QVector<StatData *>&)), ui_controller, SIGNAL(hasNewStats(QVector<StatData *>&)));
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
	QNetworkProxy proxy;
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

		proxy.setType(QNetworkProxy::Socks5Proxy);
		proxy.setHostName(proxyhost);
		proxy.setPort(proxyport.toInt());

		if (socks_dns) {
			proxy.setCapabilities(QNetworkProxy::HostNameLookupCapability);
		}

		if (proxyuser.length() > 0) {
			proxy.setUser(proxyuser);
			proxy.setPassword(proxypass);
		}

		QNetworkProxy::setApplicationProxy(proxy);
	} else {
		proxy.setType(QNetworkProxy::NoProxy);
		QNetworkProxy::setApplicationProxy(proxy);
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
