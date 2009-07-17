#include "TreeNode.h"
#include <QString>

TreeNode::TreeNode(const QString &s, TreeNode *parent)
{
	data = s;
	this->parent = parent;
}

TreeNode::~TreeNode()
{
	qDeleteAll(children);
}
