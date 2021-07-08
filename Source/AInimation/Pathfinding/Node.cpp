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

float Node::GetHCost() const
{
	return m_fHCost;
}

float Node::GetGCost() const
{
	return m_fGCost;
}

float Node::GetFCost() const
{
	return m_fHCost + m_fGCost;
}

void Node::SetNodeParent(NavNodeRef p_nodeParent)
{
	m_nodeParent = p_nodeParent;
}

void Node::SetHCost(float p_fHCost)
{
	m_fHCost = p_fHCost;
}

void Node::SetGCost(float p_fGCost)
{
	m_fGCost = p_fGCost;
}

bool Node::operator==(Node const& rightNode)
{
	if (this->GetNodeRef() == rightNode.GetNodeRef())
		return true;

	return false;
}
