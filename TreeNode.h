#ifndef TREENODE_H
#define TREENODE_H

#include <QList>
#include <QString>

class TreeNode
{
public:
	TreeNode(const QString &s = "", TreeNode *parent = 0);
	~TreeNode();

	QString data;
	TreeNode *parent;
	QList<TreeNode *>children;
};

#endif // TREENODE_H
