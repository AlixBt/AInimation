#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"

/**
 * 
 */
class AINIMATION_API Node
{
	NavNodeRef m_nodeRef;
	NavNodeRef m_nodeParent;
	float m_fHCost;
	float m_fGCost;

public:
	Node();
	Node(NavNodeRef p_nodeRef);
	~Node();

	// Getters
	NavNodeRef GetNodeRef() const;
	NavNodeRef GetNodeParent() const;
	float GetHCost() const;
	float GetGCost() const;
	float GetFCost() const;

	// Setters
	void SetNodeParent(NavNodeRef p_nodeParent);
	void SetHCost(float p_fHCost);
	void SetGCost(float p_fGCost);
	
	// Operators
	bool operator == (Node const& rightNode);
};

inline bool operator == (Node const& leftNode, Node const& rightNode)
{
	if (leftNode.GetNodeRef() == rightNode.GetNodeRef())
		return true;

	return false;
}

inline bool operator != (Node const& leftNode, Node const& rightNode)
{
	return !(leftNode == rightNode);
}
