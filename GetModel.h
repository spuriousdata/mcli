#ifndef GETMODEL_H
#define GETMODEL_H

#include "TreeModel.h"

class GetModel : public TreeModel
{
public:
	int columnCount(const QModelIndex& parent) const;
	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // GETMODEL_H
