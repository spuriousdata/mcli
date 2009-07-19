#include "mci.h"
#include "ui_mci.h"
#include "TreeNode.h"
#include "StatsModel.h"
#include "StatData.h"
#include "UIController.h"
#include "AppController.h"

#include <QtGui>

McI::McI(QWidget *parent)
	: QMainWindow(parent), m_ui(new Ui::McIClass)
{
	m_ui->setupUi(this);

	/* Add functionality doesn't work right now */
	m_ui->add_button->setEnabled(false);
	connect(m_ui->maintree, SIGNAL(collapsed(QModelIndex)), this, SLOT(resizeTreeColumns(QModelIndex)));
	connect(m_ui->maintree, SIGNAL(expanded(QModelIndex)), this, SLOT(resizeTreeColumns(QModelIndex)));
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
		m_ui->maintree->resizeColumnToContents(i);
}

void McI::displayStats(QVector<StatData *> &data)
{
	QString line;
	QStringList parts, lineItem;
	StatData *sd;
	TreeNode *rootNode, *serverNode, *statNode;
	TreeModel *model;

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
	m_ui->maintree->setModel(sortModel);
	for (int i = 0; i < model->columnCount(QModelIndex()); i++)
		m_ui->maintree->resizeColumnToContents(i);
}

McI::~McI()
{
	delete m_ui;
}
