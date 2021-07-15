#include "PathFinder.h"
#include "DrawDebugHelpers.h"
#include "Algo/Reverse.h"
#include "Kismet/KismetMathLibrary.h"
#include  "../Npcs/Irex/AIIrex.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Utility/MathUtility.h"

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

PathFinder::PathFinder(ARecastNavMesh* p_pRecastNavMesh, UNavigationSystemV1* p_pNavigationSystem) :
	m_pRecastNavMesh(p_pRecastNavMesh),
	m_pNavigationSystem(p_pNavigationSystem)
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

TArray<PathEdge> PathFinder::FunnelAlgorithm(AAIIrex* t_owner, TArray<NavNodeRef> p_aPathNodes, FVector p_vStartPosition, FVector p_vTargetPosition) const
{
	TArray<FNavigationPortalEdge> aPathPortals = FindPortalsFromPath(p_aPathNodes);
	TArray<FVector> aProcessPath;

	if (!aPathPortals.IsEmpty())
	{
		FVector vApexPortal = p_vStartPosition;
		FVector vRightPortal = aPathPortals[0].Right;
		FVector vLeftPortal = aPathPortals[0].Left;

		aProcessPath.Add(p_vStartPosition);

		int iApexIndex = 0;
		int iRightIndex = 0;
		int iLeftIndex = 0;

		for (int i = 1; i < aPathPortals.Num(); ++i)
		{
			const FVector vRight = aPathPortals[i].Right;
			const FVector vLeft = aPathPortals[i].Left;

			if (Triarea2(vApexPortal, vRightPortal, vRight) <= 0.0f)
			{
				if (EqualDistance(vApexPortal, vRightPortal) || Triarea2(vApexPortal, vLeftPortal, vRight) > 0.0f)
				{
					vRightPortal = vRight;
					iRightIndex = i;
				}
				else
				{
					ShiftPathPoint(vLeftPortal, vApexPortal, vRight);
					vApexPortal = vLeftPortal;
					iApexIndex = iLeftIndex;
					aProcessPath.Add(vApexPortal);

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
				if (EqualDistance(vApexPortal, vLeftPortal) || Triarea2(vApexPortal, vRightPortal, vLeft) < 0.0f)
				{
					vLeftPortal = vLeft;
					iLeftIndex = i;
				}
				else
				{
					ShiftPathPoint(vRightPortal, vApexPortal, vLeft);
					vApexPortal = vRightPortal;
					iApexIndex = iRightIndex;
					aProcessPath.Add(vApexPortal);

					vLeftPortal = vApexPortal;
					vRightPortal = vApexPortal;

					iLeftIndex = iApexIndex;
					iRightIndex = iApexIndex;

					i = iApexIndex;
					continue;
				}
			}
		}

		aProcessPath.Add(p_vTargetPosition);
	}

	if (!aProcessPath.IsEmpty())
	{
		SmoothPath(aProcessPath);
	}

	TArray<PathEdge> returnPath = ConvertPath(aProcessPath);
	setDirectionToNext(returnPath, t_owner->GetNPC()->GetActorForwardVector());
	return returnPath;
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

void PathFinder::ShiftPathPoint(FVector& p_vPointToAdd, FVector p_vApexPoint, FVector p_vPointChecked) const
{
	FVector vOutDistance;
	float fDistance = m_pRecastNavMesh->FindDistanceToWall(p_vPointToAdd, nullptr, 100.0f, &vOutDistance);

	if (fDistance == 0.0f)
	{
		FVector vHorizontalVector = (p_vPointChecked - p_vApexPoint);
		FVector vOrthogonalVector = FVector(1.0f, -vHorizontalVector.X / vHorizontalVector.Y, 0.0f);

		if (FVector::DotProduct(vHorizontalVector, vOrthogonalVector) == 0.0f)
		{
			FVector vPointToTest = p_vPointToAdd + (vOrthogonalVector.GetSafeNormal() * 400.0f);
			FNavLocation OutLocation;
			FVector vExtent = FVector::ZeroVector;

			bool bProjectPoint = m_pRecastNavMesh->ProjectPoint(vPointToTest, OutLocation, vExtent);

			if (bProjectPoint)
			{
				p_vPointToAdd = vPointToTest;
			}
			else
			{
				p_vPointToAdd = p_vPointToAdd + (-vOrthogonalVector.GetSafeNormal() * 400.0f);
			}
		}
	}
}

void PathFinder::SmoothPath(TArray<FVector>& p_aRoughPath) const
{
	for (int i = 1; i < p_aRoughPath.Num(); i++)
	{
		if (i == p_aRoughPath.Num() - 1)
			break;

		FVector vSegmentStart = p_aRoughPath[i - 1];
		FVector vPotentialDelete = p_aRoughPath[i];
		FVector vSegmentEnd = p_aRoughPath[i + 1];

		FVector vHitLocation;
		FSharedConstNavQueryFilter QueryFilter;
		bool bIsSegmentOnNavmesh = m_pRecastNavMesh->NavMeshRaycast(&m_pNavigationSystem->GetMainNavDataChecked(), vSegmentStart, vSegmentEnd, vHitLocation, QueryFilter);

		if (!bIsSegmentOnNavmesh)
		{
			p_aRoughPath.RemoveAt(i, 1, true);
			i--;
		}
	}
}

TArray<PathEdge> PathFinder::ConvertPath(TArray<FVector> p_aPath) const
{
	TArray<PathEdge> aPathEdge;

	if (!p_aPath.IsEmpty())
	{
		for (int i = 0; i < p_aPath.Num() - 1; i++)
		{
			aPathEdge.Add(PathEdge(p_aPath[i], p_aPath[i + 1], EBehaviorType::EBT_Normal));
		}
	}

	return aPathEdge;
}

void PathFinder::setDirectionToNext(TArray<PathEdge>& t_path, FVector t_actorForwardVector) const
{
	for (int i = 0; i < t_path.Num(); ++i)
	{
		FVector pathEdgeDirection = FVector(t_path[i].GetDestinationPosition() - t_path[i].GetSourcePosition()).GetSafeNormal(1.0f);
		FVector orthogonalToPathEdge = MathUtility::getOrthogonal2D(pathEdgeDirection).GetSafeNormal(1.0f);

		// If there is only one edge
		if (0 == t_path.Num() - 1)
		{
			t_path[i].setSourceDirection(t_actorForwardVector);

			if (FVector::DotProduct(t_actorForwardVector, orthogonalToPathEdge) > 0.0f)
			{
				orthogonalToPathEdge = -orthogonalToPathEdge;
			}

			t_path[i].setTargetDirection(orthogonalToPathEdge);
			UE_LOG(LogTemp, Warning, TEXT("One edge in path"));
			break;
		}

		if (i == 0)
		{
			t_path[i].setSourceDirection(t_actorForwardVector);

			FVector nextPathEdgeDirection = FVector(t_path[i + 1].GetDestinationPosition() - t_path[i + 1].GetSourcePosition()).GetSafeNormal(1.0f);

			t_path[i].setTargetDirection(nextPathEdgeDirection);
			UE_LOG(LogTemp, Warning, TEXT("First in path"));
		}
		else 
		{
			t_path[i].setSourceDirection(t_path[i - 1].getTargetDirection());


			if (i == t_path.Num() - 1)
			{
				if (FVector::DotProduct(t_path[i].getSourceDirection(), orthogonalToPathEdge) > 0.0f)
				{
					orthogonalToPathEdge = -orthogonalToPathEdge;
				}

				t_path[i].setTargetDirection(orthogonalToPathEdge);
			}
			else
			{
				FVector nextPathEdgeDirection = FVector(t_path[i + 1].GetDestinationPosition() - t_path[i + 1].GetSourcePosition()).GetSafeNormal(1.0f);
				t_path[i].setTargetDirection(nextPathEdgeDirection);
			}
			
			UE_LOG(LogTemp, Warning, TEXT("On the path"));
		}
	}
}

TArray<PathEdge> PathFinder::steeringCircles(TArray<PathEdge> t_path, AAIIrex* t_owner) const
{
	// We iterate through the edges
	for (int i = 0; i < t_path.Num(); ++i)
	{
		TArray<FVector> currentCircleCenters = calculateCircleCenters(t_path[i], t_owner->getTurnRadius());
		t_owner->circleCenters.Add(currentCircleCenters[0]);
		t_owner->circleCenters.Add(currentCircleCenters[1]);
	}

	return t_path;
}

TArray<FVector> PathFinder::calculateCircleCenters(PathEdge t_currentEdge, float t_turnRadius) const
{
	TArray<FVector> circleCenters;

	// Source position
	// We calculate the direction vector
	FVector directionVector = FVector(t_currentEdge.GetDestinationPosition() - t_currentEdge.GetSourcePosition()).GetSafeNormal(1.0f);

	// We calculate the orthogonal vector the source direction
	// The orthogonal vector needs to be in the same direction than the direction vector
	FVector orthogonalVector = MathUtility::getOrthogonal2D(t_currentEdge.getSourceDirection()).GetSafeNormal(1.0f);

	if (FVector::DotProduct(directionVector, orthogonalVector) < 0.0f)
	{
		orthogonalVector = -orthogonalVector;
	}

	// We scale it to have a length of the radius and the first circle center
	orthogonalVector = orthogonalVector * t_turnRadius;
	circleCenters.Add(t_currentEdge.GetSourcePosition() + orthogonalVector);

	// We do the inverse for the target position
	orthogonalVector = MathUtility::getOrthogonal2D(t_currentEdge.getTargetDirection()).GetSafeNormal(1.0f);

	if (FVector::DotProduct(-directionVector, orthogonalVector) < 0.0f)
	{
		orthogonalVector = -orthogonalVector;
	}

	orthogonalVector = orthogonalVector * t_turnRadius;
	circleCenters.Add(t_currentEdge.GetDestinationPosition() + orthogonalVector);

	return circleCenters;
}
