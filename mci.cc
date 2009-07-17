#include "mci.h"
#include "connect_dialog.h"
#include "ConfigDialog.h"
#include "ui_mci.h"
#include "TreeNode.h"
#include "StatsModel.h"
#include "StatData.h"

#include <QtGui>

class MemcacheClient;

McI::McI(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::McIClass)
{
	ui->setupUi(this);
	connect(ui->action_quick_connect, SIGNAL(triggered()), this, SLOT(openConnectDialog()));
	connect(ui->action_properties, SIGNAL(triggered()), this, SLOT(openConfigDialog()));
	connect(ui->add_button, SIGNAL(clicked()), this, SLOT(addClicked()));
	connect(ui->delete_button, SIGNAL(clicked()), this, SLOT(deleteClicked()));
	connect(ui->get_button, SIGNAL(clicked()), this, SLOT(getClicked()));
	connect(ui->flushall_button, SIGNAL(clicked()), this, SLOT(flushallClicked()));
	connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(openConnectDialog()));
}

void McI::openConfigDialog()
{
	if (!config_dialog) {
		config_dialog = new ConfigDialog(this);
	}
	config_dialog->show();
	config_dialog->raise();
	config_dialog->activateWindow();
}

void McI::openConnectDialog()
{
	if (!connect_dialog) {
		connect_dialog = new ConnectDialog(this);
	}
	connect_dialog->show();
	connect_dialog->raise();
	connect_dialog->activateWindow();
}

void McI::displayStats()
{
	StatData **data;
	QString line;
	QStringList parts, lineItem;
	StatData *sd;
	TreeNode *rootNode, *serverNode, *statNode;
	TreeModel *model;
	int i;

	data = connect_dialog->mc->stats;
	rootNode = new TreeNode();

	for (i = 0; i < connect_dialog->mc->connections.size(); i++) {
		sd = data[i];
		lineItem.clear();
		lineItem << sd->server << "" << "";
		serverNode = new TreeNode(lineItem, rootNode);
		rootNode->children.append(serverNode);
		foreach (line, sd->stats) {
			if (!line.startsWith("END")) {
				parts = line.split(" ");
				lineItem.clear();
				lineItem << "" << parts.at(1) << parts.at(2);
				statNode = new TreeNode(lineItem, serverNode);
				serverNode->children.append(statNode);
			}
		}
	}

	model = new StatsModel();
	model->setRootNode(rootNode);
	ui->maintree->setModel(model);
}

void McI::addClicked()
{
	qDebug("addClicked() called");
}

void McI::getClicked()
{
	qDebug("getClicked() called");
}

void McI::deleteClicked()
{
	qDebug("deleteClicked() called");
}
void McI::flushallClicked()
{
	qDebug("flushallClicked() called");
}

McI::~McI()
{
	delete ui;
	delete connect_dialog;
}
