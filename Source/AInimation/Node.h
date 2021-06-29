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
	float m_fGCost;
	float m_fFCost;

public:
	Node();
	Node(NavNodeRef p_nodeRef);
	~Node();

	// Getters
	NavNodeRef GetNodeRef() const;
	NavNodeRef GetNodeParent() const;
	float GetGCost() const;
	float GetFCost() const;

	// Setters
	void SetNodeParent(NavNodeRef p_nodeParent);
	void SetGCost(float p_fGCost);
	void SetFCost(float p_fFCost);

	// Overloaded operators
	bool operator==(const Node& p_nodeOther);
};
