#include "StatsModel.h"
#include <QtGui>
#include "TreeNode.h"
#include "macros.h"

int StatsModel::columnCount(const QModelIndex& UNUSED(parent)) const
{
	return 3;
}

QVariant StatsModel::data(const QModelIndex& index, int role) const
{
	TreeNode *node;
	int i;

	if (role != Qt::DisplayRole) return QVariant();

	node = nodeFromIndex(index);
	if (!node) return QVariant();

	i = index.column();
	if (i >= 0 && i < 4)
		return node->data.at(i);

	else return QVariant();
}

QVariant StatsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		if (section == 0) {
			return tr("Host");
		} else if (section == 1) {
			return tr("STAT");
		} else if (section == 2) {
			return tr("Value");
		}
	}
	return QVariant();
}

