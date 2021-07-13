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

TArray<PathEdge> PathFinder::FunnelAlgorithm(UWorld* p_pWorld, TArray<NavNodeRef> p_aPathNodes, FVector p_vStartPosition, FVector p_vTargetPosition) const
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

	return ConvertPath(aProcessPath);
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

TArray<FVector> PathFinder::createControlPoints(PathEdge t_roughPathEdge, AAIIrex* t_owner, bool t_bOneInPath, FVector lastDirectionalVector) const
{
	// We add the first control point to the array
	TArray<FVector> controlPoints;
	FVector startPoint = t_roughPathEdge.GetSourcePosition();
	FVector endPoint = t_roughPathEdge.GetDestinationPosition();
	FVector directionalEdgeVector = FVector(startPoint - endPoint).GetSafeNormal(1.0f);
	controlPoints.Add(startPoint);

	// We took care of the exception: only one edge on the path
	FVector actorForwardVector = t_owner->GetNPC()->GetActorForwardVector();
	float maxWalkSpeed = t_owner->GetNPC()->GetCharacterMovement()->MaxWalkSpeed;
	FVector secondControlPoint = startPoint + actorForwardVector * maxWalkSpeed;

	if (t_bOneInPath && lastDirectionalVector == FVector::ZeroVector)
	{
		// We add the second control point based on the speed and forward vector of the NPC
		controlPoints.Add(secondControlPoint);

		// We calculate the third control point based on the turning radius of the NPC and distance to cover
		actorForwardVector = adjustDirectionalVector(actorForwardVector, startPoint, endPoint, t_owner->getTurnRate());

		FVector thirdControlPoint = endPoint + actorForwardVector * maxWalkSpeed;
		FVector firstDirectionalVector = FVector(controlPoints[1] - startPoint).GetSafeNormal(1.0f);
		FVector secondDirectionalVector = (thirdControlPoint - endPoint).GetSafeNormal(1.0f);
		controlPoints.Add(thirdControlPoint);

		float dotProduct = FVector::DotProduct(firstDirectionalVector, secondDirectionalVector);

		UE_LOG(LogTemp, Warning, TEXT("Dot product: %f"), dotProduct);

		controlPoints.Add(endPoint);
	}

	if (!t_bOneInPath && lastDirectionalVector == FVector::ZeroVector)
	{
		// We add the second control point based on the speed and forward vector of the NPC
		controlPoints.Add(secondControlPoint);

		// We calculate the third control point based on the turning radius of the NPC and distance to cover
		actorForwardVector = adjustDirectionalVector(actorForwardVector, startPoint, endPoint, t_owner->getTurnRate());
		FVector thirdControlPoint = endPoint + actorForwardVector * maxWalkSpeed;
		controlPoints.Add(thirdControlPoint);

		// We add the fourth control point
		controlPoints.Add(endPoint);

		// We add the directional vector of the destination point for the next source point
		controlPoints.Add(FVector(controlPoints[2] - controlPoints[3]).GetSafeNormal(1.0f));
		UE_LOG(LogTemp, Warning, TEXT("i == 0"));
	}

	if (!t_bOneInPath && lastDirectionalVector != FVector::ZeroVector)
	{
		// We add the second control point based on the speed of the NPC and the last directional vector
		lastDirectionalVector = -lastDirectionalVector;
		UE_LOG(LogTemp, Warning, TEXT("last vector: %s"), *lastDirectionalVector.ToString());
		secondControlPoint = startPoint + lastDirectionalVector * maxWalkSpeed;
		controlPoints.Add(secondControlPoint);

		// We calculate the third control point based on the turning radius of the NPC and distance to cover
		actorForwardVector = adjustDirectionalVector(actorForwardVector, startPoint, endPoint, t_owner->getTurnRate());
		FVector thirdControlPoint = endPoint + actorForwardVector * maxWalkSpeed;
		controlPoints.Add(thirdControlPoint);

		// We add the fourth control point
		controlPoints.Add(endPoint);

		// We add the directional vector of the destination point for the next source point
		controlPoints.Add(FVector(controlPoints[2] - controlPoints[3]).GetSafeNormal(1.0f));
	}

	t_owner->startPoint = controlPoints[0];
	t_owner->firstControlPoint = controlPoints[1];
	t_owner->secondControlPoint = controlPoints[2];
	t_owner->endPoint = controlPoints[3];

	return controlPoints;
}

TArray<PathEdge> PathFinder::bezierSmoothing(TArray<PathEdge> t_roughPath, AAIIrex* t_owner) const
{
	TArray<FVector> controlPoints;
	TArray<FVector> pathVectorPoints;
	TArray<PathEdge> pathEdgePoints;

	// First, we create the control points
	for (int i = 0; i < t_roughPath.Num(); i++)
	{
		// Exception if there is only on edge in the path
		if (0 == t_roughPath.Num() - 1)
		{
			controlPoints = createControlPoints(t_roughPath[i], t_owner, true, FVector::ZeroVector);
			cubicCurve(pathVectorPoints, controlPoints, true);
			pathEdgePoints = ConvertPath(pathVectorPoints);
			break;
		}

		if (i == 0)
		{
			controlPoints = createControlPoints(t_roughPath[i], t_owner, false, FVector::ZeroVector);
			cubicCurve(pathVectorPoints, controlPoints, true);
		}
		else
		{
			controlPoints = createControlPoints(t_roughPath[i], t_owner, false, controlPoints[4]);		
			cubicCurve(pathVectorPoints, controlPoints, false);

			if (i == t_roughPath.Num() - 1)
			{
				pathEdgePoints = ConvertPath(pathVectorPoints);
			}
		} 
	}

	return pathEdgePoints;
}

void PathFinder::cubicCurve(TArray<FVector>& t_pathVectorPoints, TArray<FVector> t_controlPoints, bool t_bFirstEdge) const
{
	// We get some points on the curve based on the cubic curve formula
	FVector firstControlPoint = t_controlPoints[0];
	FVector secondControlPoint = t_controlPoints[1];
	FVector thirdControlPoint = t_controlPoints[2];
	FVector fourthControlPoint = t_controlPoints[3];

	// If it is the first edge, we add the start point to the array
	if (t_bFirstEdge)
	{
		t_pathVectorPoints.Add(firstControlPoint);

		for (double i = 0.1; i < 1; i += 0.1)
		{
			float x = FMath::Pow((1 - i), 3) * firstControlPoint.X + 
			          3 * FMath::Pow((1 - i), 2) * i * secondControlPoint.X +
					  3 * FMath::Pow((1 - i), 2) * i * thirdControlPoint.X +
					  FMath::Pow(i, 3) * fourthControlPoint.X;

			float y = FMath::Pow((1 - i), 3) * firstControlPoint.Y + 
			          3 * FMath::Pow((1 - i), 2) * i * secondControlPoint.Y +
					  3 * FMath::Pow((1 - i), 2) * i * thirdControlPoint.Y +
					  FMath::Pow(i, 3) * fourthControlPoint.Y;

			t_pathVectorPoints.Add(FVector(x, y, 0.0f));
		}

		t_pathVectorPoints.Add(fourthControlPoint);
	}
	else
	{
		for (double i = 0.2; i < 1; i += 0.2)
		{
			float x = FMath::Pow((1 - i), 3) * firstControlPoint.X + 
			          3 * FMath::Pow((1 - i), 2) * i * secondControlPoint.X +
					  3 * FMath::Pow((1 - i), 2) * i * thirdControlPoint.X +
					  FMath::Pow(i, 3) * fourthControlPoint.X;

			float y = FMath::Pow((1 - i), 3) * firstControlPoint.Y + 
			          3 * FMath::Pow((1 - i), 2) * i * secondControlPoint.Y +
					  3 * FMath::Pow((1 - i), 2) * i * thirdControlPoint.Y +
					  FMath::Pow(i, 3) * fourthControlPoint.Y;

			t_pathVectorPoints.Add(FVector(x, y, 0.0f));
		}

		t_pathVectorPoints.Add(fourthControlPoint);
	}
}

FVector PathFinder::adjustDirectionalVector(FVector t_startDirection, FVector t_startPoint, FVector t_endPoint, float t_turnRate) const
{
	float startPointX = t_startPoint.X;
	float startPointY = t_startPoint.Y;

	float endPointX = t_endPoint.X;
	float endPointY = t_endPoint.Y;

	if ((endPointX >= (startPointX - t_turnRate) && endPointX <= (startPointX + t_turnRate)) || 
	    (endPointY >= (startPointY - t_turnRate) && endPointY <= (startPointY + t_turnRate)))
	{
		FVector endDirection = MathUtility::getCrossProductVector2D(t_startDirection, 0.38f).GetSafeNormal(1.0f);

		if (FVector::DotProduct(t_startDirection, endDirection) >= 0.0f)
		{
			return -endDirection;
		}

		return endDirection;
	}

	if (endPointX <= (startPointX - t_turnRate) || 
	    endPointY <= (startPointY - t_turnRate))
	{
		FVector endDirection = MathUtility::getCrossProductVector2D(t_startDirection, 0.71f).GetSafeNormal(1.0f);

		if (FVector::DotProduct(t_startDirection, endDirection) <= 0.0f)
		{
			return -endDirection;
		}

		return endDirection;
	}

	if (endPointX >= (startPointX - t_turnRate) || 
	    endPointY >= (startPointY - t_turnRate))
	{
		FVector endDirection = MathUtility::getCrossProductVector2D(t_startDirection, -0.71f).GetSafeNormal(1.0f);

		if (FVector::DotProduct(t_startDirection, endDirection) <= 0.0f)
		{
			return -endDirection;
		}

		return endDirection;
	}

	return FVector::ZeroVector;
}
