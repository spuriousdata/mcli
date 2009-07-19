#include "AppController.h"
#include "UIController.h"
#include "MemcacheClient.h"
#include "ConnectDialog.h"
#include "KeyValuePrompt.h"

AppController::AppController()
{
	ui_controller = new UIController();
	memcache = new MemcacheClient();

	connect(ui_controller, SIGNAL(doConnect()), this, SLOT(mcConnect()));
	connect(ui_controller,SIGNAL(doAdd()), this, SLOT(addItem()));
	connect(ui_controller, SIGNAL(doGet()), this, SLOT(getItem()));
	connect(ui_controller, SIGNAL(doDelete()), this, SLOT(deleteItem()));
	connect(memcache, SIGNAL(hasNewStats(QVector<StatData *>&)), ui_controller, SIGNAL(hasNewStats(QVector<StatData *>&)));
}

void AppController::mcConnect()
{
	memcache->mc_connect(ui_controller->getConnectDialog()->servers);
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
