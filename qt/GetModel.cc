#include "GetModel.h"
#include "macros.h"

int GetModel::columnCount(const QModelIndex& UNUSED(parent)) const
{
	return 5;
}

QVariant GetModel::data(const QModelIndex& index, int role) const
{
	TreeNode *node;
	int i;

	if (role != Qt::DisplayRole) return QVariant();

	node = nodeFromIndex(index);
	if (!node) return QVariant();

	i = index.column();
	if (i >= 0 && i < 5)
		return node->data.at(i);

	else return QVariant();
}

QVariant GetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0:
				return tr("Host");
			case 1:
				return tr("Key");
			case 2:
				return tr("Flags");
			case 3:
				return tr("Bytes");
			case 4:
				return tr("Value");
			default:
				return QVariant();
		}
	}
	return QVariant();
}
