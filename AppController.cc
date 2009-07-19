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
	memcache = new MemcacheClient();

	connect(ui_controller, SIGNAL(doConnect()), this, SLOT(mcConnect()));
	connect(ui_controller,SIGNAL(doAdd()), this, SLOT(addItem()));
	connect(ui_controller, SIGNAL(doGet()), this, SLOT(getItem()));
	connect(ui_controller, SIGNAL(doDelete()), this, SLOT(deleteItem()));
	connect(memcache, SIGNAL(hasNewStats(QVector<StatData *>&)), ui_controller, SIGNAL(hasNewStats(QVector<StatData *>&)));
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
