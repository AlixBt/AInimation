#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationSystem.h"
#include "Node.h"

/**
 * 
 */
class AINIMATION_API PathFinder
{
	ARecastNavMesh* m_pRecastNavMesh;

	// Functions
	Node GetLowestFCostNode(TArray<Node> p_aOpenList) const;

public:
	PathFinder(ARecastNavMesh* p_pRecastNavMesh);
	~PathFinder();

	float CalculateHeuristic(NavNodeRef p_sourceNode, NavNodeRef p_targetNode) const;
	void AStar(UWorld* p_pWorld, NavNodeRef p_startNode, NavNodeRef p_targetNode);
	TArray<NavNodeRef> RetracePath(Node p_startNode, Node p_targetNode, TArray<Node> p_aClosedList) const;
	TArray<Node> GetNodeNeighbours(NavNodeRef p_nodeRef) const;

};
