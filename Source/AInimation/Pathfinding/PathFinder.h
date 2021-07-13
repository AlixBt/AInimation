#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationSystem.h"
#include "Node.h"
#include "PathEdge.h"

class AAIIrex;

/**
 * 
 */
class AINIMATION_API PathFinder
{
	ARecastNavMesh* m_pRecastNavMesh;
	UNavigationSystemV1* m_pNavigationSystem;

	// Functions
	Node GetLowestFCostNode(TArray<Node> p_aOpenList) const;

public:
	PathFinder(ARecastNavMesh* p_pRecastNavMesh, UNavigationSystemV1* p_pNavigationSystem);
	~PathFinder();

	float CalculateHeuristic(NavNodeRef p_sourceNode, NavNodeRef p_targetNode) const;
	TArray<NavNodeRef> AStar(UWorld* p_pWorld, NavNodeRef p_startNode, NavNodeRef p_targetNode);
	TArray<NavNodeRef> RetracePath(Node p_startNode, Node p_targetNode, TArray<Node> p_aClosedList) const;
	TArray<Node> GetNodeNeighbours(NavNodeRef p_nodeRef) const;

	TArray<PathEdge> FunnelAlgorithm(UWorld* p_pWorld, TArray<NavNodeRef> p_aPathNodes, FVector p_vStartPosition, FVector p_vTargetPosition) const;
	TArray<FNavigationPortalEdge> FindPortalsFromPath(TArray<NavNodeRef> p_aPathNodes) const;
	float Triarea2(FVector p_vApex, FVector p_vCurrentVector, FVector p_vCandidateVector) const;
	bool EqualDistance(FVector p_vCurrent, FVector p_vToCheck) const;
	void ShiftPathPoint(FVector& p_vPointToAdd, FVector p_vApexPoint, FVector p_vPointChecked) const;
	void SmoothPath(TArray<FVector>& p_aRoughPath) const;
	TArray<PathEdge> ConvertPath(TArray<FVector> p_aPath) const;

	TArray<FVector> createControlPoints(PathEdge t_roughPathEdge, AAIIrex* t_owner, bool t_bOneInPath, FVector lastDirectionalVector) const;
	TArray<PathEdge> bezierSmoothing(TArray<PathEdge> t_roughPath, AAIIrex* t_owner) const;
	void cubicCurve(TArray<FVector>& t_pathVectorPoints, TArray<FVector> t_controlPoints, bool t_bFirstEdge) const;
	FVector adjustDirectionalVector(FVector t_startDirection, FVector t_startPoint, FVector t_endPoint, float t_turnRate) const;
};