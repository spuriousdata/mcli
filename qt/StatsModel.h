#ifndef STATSMODEL_H
#define STATSMODEL_H

#include <QModelIndex>
#include <QVariant>
#include <QtCore/Qt>
#include "TreeNode.h"
#include "TreeModel.h"

class StatsModel : public TreeModel
{
public:
	int columnCount(const QModelIndex& parent) const;
	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // STATSMODEL_H
