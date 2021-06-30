#include "PathFinder.h"
#include "DrawDebugHelpers.h"
#include "Algo/Reverse.h"

Node PathFinder::GetLowestFCostNode(TArray<Node> p_aOpenList) const
{
	Node bestNode = p_aOpenList[0];

	// We search through the lowest F cost through the array
	for (int i = 0; i < p_aOpenList.Num(); i++)
	{
		if (p_aOpenList[i].GetFCost() <= bestNode.GetFCost() && p_aOpenList[i].GetHCost() < bestNode.GetHCost())
		{
			bestNode = p_aOpenList[i];
		}
	}

	return bestNode;
}

PathFinder::PathFinder(ARecastNavMesh* p_pRecastNavMesh) :
	m_pRecastNavMesh(p_pRecastNavMesh)
{
}

PathFinder::~PathFinder()
{
}

float PathFinder::CalculateHeuristic(NavNodeRef p_sourceNode, NavNodeRef p_targetNode) const
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

void PathFinder::AStar(UWorld* p_pWorld, NavNodeRef p_startNode, NavNodeRef p_targetNode)
{
	TArray<Node> aOpenList;
	TArray<Node> aClosedList;

	// We add the start node to the open list 
	Node startNode = Node(p_startNode);
	Node targetNode = Node(p_targetNode);

	startNode.SetHCost(CalculateHeuristic(p_startNode, p_targetNode));
	startNode.SetGCost(0.0f);
	startNode.SetNodeParent(startNode.GetNodeRef());
	aOpenList.Add(startNode);

	while (!aOpenList.IsEmpty())
	{
		Node currentNode = GetLowestFCostNode(aOpenList);
		aOpenList.Remove(currentNode);
		aClosedList.Add(currentNode);

		if (currentNode == targetNode)
		{
			TArray<NavNodeRef> aPath = RetracePath(startNode, currentNode, aClosedList);

			for (int i = 0; i < aPath.Num(); i++)
			{
				FVector vCenterPosition;
				m_pRecastNavMesh->GetPolyCenter(aPath[i], vCenterPosition);
				DrawDebugBox(p_pWorld, vCenterPosition, FVector(50.0f, 50.0f, 50.0f), FColor::Purple, false, -1.0f, 0, 5.0f);
			}

			return;
		}

		// We get all the neighbours to the current node
		for (Node neighbour : GetNodeNeighbours(currentNode.GetNodeRef()))
		{
			if (aClosedList.Contains(neighbour))
			{
				continue;
			}

			float fNewGCost = currentNode.GetGCost() + CalculateHeuristic(currentNode.GetNodeRef(), neighbour.GetNodeRef());

			if (fNewGCost < neighbour.GetGCost() || !aOpenList.Contains(neighbour))
			{
				neighbour.SetHCost(CalculateHeuristic(neighbour.GetNodeRef(), targetNode.GetNodeRef()));
				neighbour.SetGCost(fNewGCost);
				neighbour.SetNodeParent(currentNode.GetNodeRef());

				if (!aOpenList.Contains(neighbour))
					aOpenList.Add(neighbour);
			}
		}
	}
}

TArray<NavNodeRef> PathFinder::RetracePath(Node p_startNode, Node p_targetNode, TArray<Node> p_aClosedList) const
{
	TArray<NavNodeRef> aReturnPath;
	Node currentNode = p_targetNode;

	while (currentNode != p_startNode)
	{
		aReturnPath.Add(currentNode.GetNodeRef());

		for (int i = 0; i < p_aClosedList.Num(); i++)
		{
			if (currentNode.GetNodeParent() == p_aClosedList[i].GetNodeRef())
			{
				currentNode = p_aClosedList[i];
			}
		}
	}

	Algo::Reverse(aReturnPath);
	return aReturnPath;
}

TArray<Node> PathFinder::GetNodeNeighbours(NavNodeRef p_nodeRef) const
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
