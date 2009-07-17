#include "TreeModel.h"
#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "TreeNode.h"

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent)
{
	rootNode = 0;
}

void TreeModel::setRootNode(TreeNode *node)
{
	delete rootNode;
	rootNode = node;
	reset();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
	TreeNode *parentNode, *child;

	if (!rootNode || row < 0 || column < 0)
		return QModelIndex();
	parentNode = nodeFromIndex(parent);
	child = parentNode->children.value(row);
	if (!child)
		return QModelIndex();
	return createIndex(row, column, child);
}

TreeNode *TreeModel::nodeFromIndex(const QModelIndex& index) const
{
	if (index.isValid())
		return static_cast<TreeNode *>(index.internalPointer());
	else return rootNode;
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
	TreeNode *parentNode;

	if (parent.column() > 0) return 0;

	parentNode = nodeFromIndex(parent);
	if (!parentNode) return 0;
	return parentNode->children.count();
}

QModelIndex TreeModel::parent(const QModelIndex& child) const
{
	TreeNode *parentNode, *grandparentNode, *node;
	int row;

	node = nodeFromIndex(child);
	if (!node)
		return QModelIndex();
	parentNode = node->parent;
	if (!parentNode)
		return QModelIndex();
	grandparentNode = parentNode->parent;
	if (!grandparentNode)
		return QModelIndex();

	row = grandparentNode->children.indexOf(parentNode);
	return createIndex(row, 0, parentNode);
}

TreeModel::~TreeModel()
{
	delete rootNode;
}
