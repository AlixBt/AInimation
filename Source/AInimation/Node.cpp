#include "Node.h"

Node::Node()
{
}

Node::Node(NavNodeRef p_nodeRef) :
	m_nodeRef(p_nodeRef)
{
}

Node::~Node()
{
}

NavNodeRef Node::GetNodeRef() const
{
	return m_nodeRef;
}

NavNodeRef Node::GetNodeParent() const
{
	return m_nodeParent;
}

float Node::GetGCost() const
{
	return m_fGCost;
}

float Node::GetFCost() const
{
	return m_fFCost;
}

void Node::SetNodeParent(NavNodeRef p_nodeParent)
{
	m_nodeParent = p_nodeParent;
}

void Node::SetGCost(float p_fGCost)
{
	m_fGCost = p_fGCost;
}

void Node::SetFCost(float p_fFCost)
{
	m_fFCost = p_fFCost;
}

bool Node::operator==(const Node& p_nodeOther)
{
	return (m_nodeRef == p_nodeOther.GetNodeRef());
}
