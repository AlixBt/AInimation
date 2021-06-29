

#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationSystem.h"
#include "Node.h"

/**
 * 
 */
class AINIMATION_API AStar
{
	ARecastNavMesh* m_pRecastNavMesh;

	// Functions
	Node GetLowestFCostNode(TArray<Node> p_aOpenList) const;

public:
	AStar(ARecastNavMesh* p_pRecastNavMesh);
	~AStar();

	float CalculateHeuristic(NavNodeRef p_sourceNode, NavNodeRef p_targetNode) const;
	void SearchShortestPathOfNodes(UWorld* p_pWorld, NavNodeRef p_startNode, NavNodeRef p_targetNode);
	TArray<Node> GetNodeNeighbours(NavNodeRef p_nodeRef) const;
};
