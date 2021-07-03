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
	TArray<NavNodeRef> AStar(UWorld* p_pWorld, NavNodeRef p_startNode, NavNodeRef p_targetNode);
	TArray<NavNodeRef> RetracePath(Node p_startNode, Node p_targetNode, TArray<Node> p_aClosedList) const;
	TArray<Node> GetNodeNeighbours(NavNodeRef p_nodeRef) const;

	TArray<FVector> FunnelAlgorithm(UWorld* p_pWorld, TArray<NavNodeRef> p_aPathNodes, FVector p_vStartPosition, FVector p_vTargetPosition) const;
	TArray<FNavigationPortalEdge> FindPortalsFromPath(TArray<NavNodeRef> p_aPathNodes) const;
	float Triarea2(FVector p_vApex, FVector p_vCurrentVector, FVector p_vCandidateVector) const;
	bool EqualDistance(FVector p_vCurrent, FVector p_vToCheck) const;
	void ShiftPathPoint(FVector& p_vPointToAdd, FVector p_vApexPoint, FVector p_vPointChecked) const;
	void SmoothPath(TArray<FVector>& p_aRoughPath) const;
};