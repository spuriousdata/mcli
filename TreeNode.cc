#include "TreeNode.h"
#include <QString>

TreeNode::TreeNode(const QStringList &l, TreeNode *parent)
{
	data = l;
	this->parent = parent;
}

TreeNode::~TreeNode()
{
	qDeleteAll(children);
}
