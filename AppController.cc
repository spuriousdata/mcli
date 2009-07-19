#include "AppController.h"
#include "UIController.h"
#include "MemcacheClient.h"
#include "ConnectDialog.h"
#include "KeyValuePrompt.h"
#include "HostEntry.h"
#include <QSettings>
#include <QVector>

AppController::AppController()
{
	ui_controller = new UIController(this);
	memcache = new MemcacheClient(this);

	connect(ui_controller, SIGNAL(doConnect()), this, SLOT(mcConnect()));
	connect(ui_controller,SIGNAL(doAdd()), this, SLOT(addItem()));
	connect(ui_controller, SIGNAL(doGet()), this, SLOT(getItem()));
	connect(ui_controller, SIGNAL(doDelete()), this, SLOT(deleteItem()));
	connect(ui_controller, SIGNAL(doFlushAll()), memcache, SLOT(flushAll()));
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

void AppController::run()
{
	ui_controller->showMainWindow();
}

AppController::~AppController()
{
	delete ui_controller;
	delete memcache;
}
