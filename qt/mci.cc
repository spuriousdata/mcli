#include "mci.h"
#include "ui_mci.h"
#include "TreeNode.h"
#include "StatsModel.h"
#include "GetModel.h"
#include "StatData.h"
#include "GetData.h"
#include "GetItem.h"
#include "UIController.h"
#include "AppController.h"

#include <QtGui>

McI::McI(QWidget *parent)
	: QMainWindow(parent), m_ui(new Ui::McIClass)
{
	m_ui->setupUi(this);

	/* Add functionality doesn't work right now */
	m_ui->add_button->setEnabled(false);
	connect(m_ui->statsTreeView, SIGNAL(collapsed(QModelIndex)), this, SLOT(resizeTreeColumns(QModelIndex)));
	connect(m_ui->statsTreeView, SIGNAL(expanded(QModelIndex)), this, SLOT(resizeTreeColumns(QModelIndex)));
	connect(m_ui->retTreeView, SIGNAL(collapsed(QModelIndex)), this, SLOT(resizeTreeColumns(QModelIndex)));
	connect(m_ui->retTreeView, SIGNAL(expanded(QModelIndex)), this, SLOT(resizeTreeColumns(QModelIndex)));
}

void McI::setInputEnabled(bool isEnabled)
{
	m_ui->get_button->setEnabled(isEnabled);
	m_ui->delete_button->setEnabled(isEnabled);
	m_ui->flushall_button->setEnabled(isEnabled);
}

Ui::McIClass *McI::ui() const
{
	return m_ui;
}

void McI::resizeTreeColumns(const QModelIndex& index)
{
	for (int i = 0; i < index.model()->columnCount(QModelIndex()); i++)
		m_ui->statsTreeView->resizeColumnToContents(i);
}

void McI::displayGet(QVector<GetData *> &data)
{
	QStringList parts, lineItem;
	QString line;
	TreeNode *rootNode, *serverNode, *getNode;
	GetModel *model;
	GetData *gd;
	GetItem *item;

	m_ui->mainTabs->setCurrentIndex(1);
	if (m_ui->retTreeView->model())
		delete m_ui->retTreeView->model();

	rootNode = new TreeNode();

	foreach (gd, data) {
		if (gd == NULL) continue;
		lineItem.clear();
		lineItem << gd->server << "" << "" << "" << "";
		serverNode = new TreeNode(lineItem, rootNode);
		rootNode->children.append(serverNode);
		foreach (item, gd->items) {
			lineItem.clear();
			lineItem << "" << item->key << item->flags
					<< QString("%1").arg(item->data.length()) << item->data;
			getNode = new TreeNode(lineItem, serverNode);
			serverNode->children.append(getNode);
		}
	}

	model = new GetModel();
	model->setRootNode(rootNode);
	m_ui->retTreeView->setModel(model);
	for (int i = 0; i < model->columnCount(QModelIndex()); i++)
		m_ui->retTreeView->resizeColumnToContents(i);

	m_ui->retTreeView->expandAll();

}

void McI::displayStats(QVector<StatData *> &data)
{
	QString line;
	QStringList parts, lineItem;
	StatData *sd;
	TreeNode *rootNode, *serverNode, *statNode;
	StatsModel *model;

	m_ui->mainTabs->setCurrentIndex(0);
	if (m_ui->statsTreeView->model()) {
		delete dynamic_cast<QSortFilterProxyModel *>(m_ui->statsTreeView->model())->sourceModel();
		delete m_ui->statsTreeView->model();
	}

	rootNode = new TreeNode();

	foreach (sd, data) {
		if (sd == NULL) continue;
		lineItem.clear();
		lineItem << sd->server << "" << "";
		serverNode = new TreeNode(lineItem, rootNode);
		rootNode->children.append(serverNode);
		foreach (line, sd->stats) {
			if (!line.startsWith("END")) {
				parts = line.split(" ");
				if (parts.size() < 3) continue;
				lineItem.clear();
				lineItem << "" << parts.at(1) << parts.at(2);
				statNode = new TreeNode(lineItem, serverNode);
				serverNode->children.append(statNode);
			}
		}
	}

	model = new StatsModel();
	model->setRootNode(rootNode);
	QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(this);
	sortModel->setSourceModel(model);
	m_ui->statsTreeView->setModel(sortModel);
	for (int i = 0; i < model->columnCount(QModelIndex()); i++)
		m_ui->statsTreeView->resizeColumnToContents(i);
}

McI::~McI()
{
	delete m_ui;
}
