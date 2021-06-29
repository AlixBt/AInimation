#include "AStar.h"
#include "DrawDebugHelpers.h"

Node AStar::GetLowestFCostNode(TArray<Node> p_aOpenList) const
{
	int iNodeIndex = -1;
	float fBestFCost = 10000000.0f;
	float fBestGCost = 10000000.0f;

	// We search through the lowest F cost through the array
	for (int i = 0; i < p_aOpenList.Num(); i++)
	{
		if (p_aOpenList[i].GetFCost() <= fBestFCost && p_aOpenList[i].GetGCost() < fBestGCost)
		{
			iNodeIndex = i;
			fBestGCost = p_aOpenList[i].GetGCost();
			fBestFCost = p_aOpenList[i].GetFCost();
		}
	}

	if (iNodeIndex != -1)
		return p_aOpenList[iNodeIndex];

	return Node();
}

AStar::AStar(ARecastNavMesh* p_pRecastNavMesh) :
	m_pRecastNavMesh(p_pRecastNavMesh)
{
}

AStar::~AStar()
{
}

float AStar::CalculateHeuristic(NavNodeRef p_sourceNode, NavNodeRef p_targetNode) const
{
	// We get the center of each node
	FVector vSourceNodeCenter;
	FVector vTargetNodeCenter;

	m_pRecastNavMesh->GetPolyCenter(p_sourceNode, vSourceNodeCenter);
	m_pRecastNavMesh->GetPolyCenter(p_targetNode, vTargetNodeCenter);

	// We calculate the distance between the two
	FVector vDistanceFromSourceToTarget = vTargetNodeCenter - vSourceNodeCenter;

	return vDistanceFromSourceToTarget.Size();
}

void AStar::SearchShortestPathOfNodes(UWorld* p_pWorld, NavNodeRef p_startNode, NavNodeRef p_targetNode)
{
	TArray<Node> aOpenList;
	TArray<Node> aClosedList;

	// We add the start node to the open list 
	Node startNode = Node(p_startNode);
	Node targetNode = Node(p_targetNode);

	startNode.SetFCost(1.0f);
	aOpenList.Add(startNode);

	while (!aOpenList.IsEmpty())
	{
		Node currentNode = GetLowestFCostNode(aOpenList);
		aOpenList.Remove(currentNode);
		aClosedList.Add(currentNode);

		if (currentNode == targetNode)
		{
			return;
		}

		// We get all the neighbours to the current node
		for (Node neighbour : GetNodeNeighbours(currentNode.GetNodeRef()))
		{
			FVector vCenterPosition;
			m_pRecastNavMesh->GetPolyCenter(neighbour.GetNodeRef(), vCenterPosition);

			DrawDebugBox(p_pWorld, vCenterPosition, FVector(100.0f, 100.0f, 100.0f), FColor::Orange, false, -1.0f, 0, 5.0f);
		}
	}
}

TArray<Node> AStar::GetNodeNeighbours(NavNodeRef p_nodeRef) const
{
	TArray<NavNodeRef> aOutNodeRef;
	m_pRecastNavMesh->GetPolyNeighbors(p_nodeRef, aOutNodeRef);

	if (!aOutNodeRef.IsEmpty())
	{
		TArray<Node> aOutNode;
		for (int i = 0; i < aOutNodeRef.Num(); i++)
		{
			aOutNode.Add(Node(aOutNodeRef[i]));
		}

		return aOutNode;
	}
	return TArray<Node>();
}
