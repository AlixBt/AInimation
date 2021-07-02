#include "PathFinder.h"
#include "DrawDebugHelpers.h"
#include "Algo/Reverse.h"
#include "Kismet/KismetMathLibrary.h"

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

TArray<NavNodeRef> PathFinder::AStar(UWorld* p_pWorld, NavNodeRef p_startNode, NavNodeRef p_targetNode)
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
			return aPath;
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

	return TArray<NavNodeRef>();
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

	aReturnPath.Add(p_startNode.GetNodeRef());
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

TArray<FVector> PathFinder::FunnelAlgorithm(UWorld* p_pWorld, TArray<NavNodeRef> p_aPathNodes, FVector p_vStartPosition, FVector p_vTargetPosition) const
{
	TArray<FNavigationPortalEdge> aPathPortals = FindPortalsFromPath(p_aPathNodes);
	TArray<FVector> aReturnPath;

	if (!aPathPortals.IsEmpty())
	{
		FVector vApexPortal = p_vStartPosition;
		FVector vRightPortal = aPathPortals[0].Right;
		FVector vLeftPortal = aPathPortals[0].Left;

		aReturnPath.Add(p_vStartPosition);

		int iApexIndex = 0;
		int iRightIndex = 0;
		int iLeftIndex = 0;

		for (int i = 1; i < aPathPortals.Num(); ++i)
		{
			UE_LOG(LogTemp, Display, TEXT("Index: %i"), i);
			const FVector vRight = aPathPortals[i].Right;
			const FVector vLeft = aPathPortals[i].Left;

			if (Triarea2(vApexPortal, vRightPortal, vRight) <= 0.0f)
			{
				UE_LOG(LogTemp, Error, TEXT("Triare2: %f"), Triarea2(vApexPortal, vRightPortal, vRight));

				if (EqualDistance(vApexPortal, vRightPortal) || Triarea2(vApexPortal, vLeftPortal, vRight) > 0.0f)
				{
					vRightPortal = vRight;
					iRightIndex = i;
					UE_LOG(LogTemp, Error, TEXT("Bool distance: %s"), (EqualDistance(vApexPortal, vRightPortal) ? TEXT("true") : TEXT("false")));
					UE_LOG(LogTemp, Error, TEXT("Triare2: %f"), Triarea2(vApexPortal, vLeftPortal, vRight));
					UE_LOG(LogTemp, Warning, TEXT("Right updated"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Bool distance: %s"), (EqualDistance(vApexPortal, vRightPortal) ? TEXT("true") : TEXT("false")));
					UE_LOG(LogTemp, Error, TEXT("Triare2: %f"), Triarea2(vApexPortal, vLeftPortal, vRight));

					vApexPortal = vLeftPortal;
					iApexIndex = iLeftIndex;
					aReturnPath.Add(vApexPortal);
					UE_LOG(LogTemp, Warning, TEXT("Left new portal"));

					vLeftPortal = vApexPortal;
					vRightPortal = vApexPortal;
					iLeftIndex = iApexIndex;
					iRightIndex = iApexIndex;

					i = iApexIndex;
					continue;
				}
			}

			if (Triarea2(vApexPortal, vLeftPortal, vLeft) >= 0.0f)
			{
				UE_LOG(LogTemp, Error, TEXT("Triare2: %f"), Triarea2(vApexPortal, vLeftPortal, vLeft));

				if (EqualDistance(vApexPortal, vLeftPortal) || Triarea2(vApexPortal, vRightPortal, vLeft) < 0.0f)
				{
					vLeftPortal = vLeft;
					iLeftIndex = i;
					UE_LOG(LogTemp, Error, TEXT("Bool distance: %s"), (EqualDistance(vApexPortal, vLeftPortal) ? TEXT("true") : TEXT("false")));
					UE_LOG(LogTemp, Error, TEXT("Triare2: %f"), Triarea2(vApexPortal, vRightPortal, vLeft));
					UE_LOG(LogTemp, Warning, TEXT("Left updated"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Bool distance: %s"), (EqualDistance(vApexPortal, vLeftPortal) ? TEXT("true") : TEXT("false")));
					UE_LOG(LogTemp, Error, TEXT("Triare2: %f"), Triarea2(vApexPortal, vRightPortal, vLeft));

					vApexPortal = vRightPortal;
					iApexIndex = iRightIndex;
					aReturnPath.Add(vApexPortal);
					UE_LOG(LogTemp, Warning, TEXT("Right new portal"));

					vLeftPortal = vApexPortal;
					vRightPortal = vApexPortal;

					iLeftIndex = iApexIndex;
					iRightIndex = iApexIndex;

					i = iApexIndex;
					continue;
				}
			}

			//DrawDebugBox(p_pWorld, aPathPortals[i].Right, FVector(20.0f, 20.0f, 20.0f), FColor::Green, false, -1.0f, 0, 2.0f);
			//DrawDebugBox(p_pWorld, aPathPortals[i].Left, FVector(20.0f, 20.0f, 20.0f), FColor::Red, false, -1.0f, 0, 2.0f);
		}

		
		
		aReturnPath.Add(p_vTargetPosition);
		UE_LOG(LogTemp, Warning, TEXT("Last point added"));
		

		/*DrawDebugLine(p_pWorld, vApexPortal, aPathPortals[0].Right, FColor::Green);
		DrawDebugLine(p_pWorld, vApexPortal, aPathPortals[1].Right, FColor::Red);
		GEngine->AddOnScreenDebugMessage(0, -1.0f, FColor::Purple, FString::Printf(TEXT("CrossProduct cost: %f"), Triarea2(vApexPortal, aPathPortals[0].Right, aPathPortals[1].Right)));*/
	}

	return aReturnPath;
}

TArray<FNavigationPortalEdge> PathFinder::FindPortalsFromPath(TArray<NavNodeRef> p_aPathNodes) const
{
	TArray<FNavigationPortalEdge> aReturnPathPortals;
	TArray<FNavigationPortalEdge> aNeighboursNodePortals;

	for (int i = 0; i < p_aPathNodes.Num() - 1; i++)
	{
		m_pRecastNavMesh->GetPolyNeighbors(p_aPathNodes[i],	aNeighboursNodePortals);

		for (int j = 0; j < aNeighboursNodePortals.Num(); j++)
		{
			if (aNeighboursNodePortals[j].ToRef == p_aPathNodes[i + 1])
			{
				aReturnPathPortals.Add(aNeighboursNodePortals[j]);
				break;
			}
		}
	}

	if (!aReturnPathPortals.IsEmpty())
		return aReturnPathPortals;

	return TArray<FNavigationPortalEdge>();
}

float PathFinder::Triarea2(FVector p_vApex, FVector p_vCurrentVector, FVector p_vCandidateVector) const
{
	FVector vObserver = FVector(p_vCurrentVector - p_vApex).GetSafeNormal();
	FVector vCandidate = FVector(p_vCandidateVector - p_vApex).GetSafeNormal();

	const float fAx = p_vCurrentVector[0] - p_vApex[0];
	const float fAy = p_vCurrentVector[1] - p_vApex[1];
	const float fBx = p_vCandidateVector[0] - p_vApex[0];
	const float fBy = p_vCandidateVector[1] - p_vApex[1];

	return fBx * fAy - fAx * fBy;
}

bool PathFinder::EqualDistance(FVector p_vCurrent, FVector p_vToCheck) const
{
	const float fConstant = 0.001f * 0.001f;
	return FVector(p_vToCheck - p_vCurrent).Size() < fConstant;
}
