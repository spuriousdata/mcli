#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>
#include <QVariant>
#include <QtCore/Qt>
#include "TreeNode.h"

class TreeModel : public QAbstractItemModel
{
public:
	TreeModel(QObject *parent = 0);
	~TreeModel();

	void setRootNode(TreeNode *node);

	QModelIndex index(int row, int column, const QModelIndex& parent) const;
	QModelIndex parent(const QModelIndex& child) const;

	int rowCount(const QModelIndex& parent) const;
	virtual int columnCount(const QModelIndex& parent) const = 0;
	virtual QVariant data(const QModelIndex& index, int role) const = 0;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const = 0;

protected:
	TreeNode *nodeFromIndex(const QModelIndex& index) const;
	TreeNode *rootNode;
};

#endif // TREEMODEL_H
