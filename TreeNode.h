#ifndef TREENODE_H
#define TREENODE_H

#include <QList>
#include <QStringList>

class TreeNode
{
public:
	TreeNode(TreeNode *_parent = 0) : parent(_parent) {}
	TreeNode(const QStringList &l, TreeNode *parent = 0);
	~TreeNode();

	QStringList data;
	TreeNode *parent;
	QList<TreeNode *>children;
};

#endif // TREENODE_H
