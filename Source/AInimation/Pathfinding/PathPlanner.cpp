#include "PathPlanner.h"
#include "../Npcs/Irex/AIIrex.h"
#include "NavMesh/RecastNavMesh.h"
#include "DrawDebugHelpers.h"
#include "PathFinder.h"
#include "PathEdge.h"

PathPlanner::PathPlanner(AAIIrex* p_pOwner) :
	m_pOwner(p_pOwner),
	m_pRecastNavMesh(nullptr),
	m_pNavigationSystem(nullptr),
	m_vDestinationToReach(FVector(0.0f, 0.0f, 0.0f))
{
}

PathPlanner::~PathPlanner()
{
}

void PathPlanner::InitializeNavMesh(UNavigationSystemV1* p_pNavigationSystem)
{
	// We get the navmesh, thank to the navigation system provided by the AI controller
	if (p_pNavigationSystem)
	{
		m_pNavigationSystem = p_pNavigationSystem;
		m_pRecastNavMesh = Cast<ARecastNavMesh>(&m_pNavigationSystem->GetMainNavDataChecked());
	}

	if (m_pRecastNavMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("PathPlanner::InitializeNavMesh() - Recast nav mesh found"));
	}
}

NavNodeRef PathPlanner::GetClosestNodeToPosition(FVector p_vPosition) const
{
	// We catch the closest node on the navmesh, to our position
	NavNodeRef closestNode = m_pRecastNavMesh->FindNearestPoly(p_vPosition, FVector(50.0f, 50.0f, 50.0f));

	if (closestNode != INVALID_NAVNODEREF)
	{
		return closestNode;
	}

	// Position outside of the navmesh
	return -1;
}

bool PathPlanner::CreatePathToPosition(FVector p_vTargetPosition, TArray<PathEdge>& p_aPath)
{
	m_vDestinationToReach = p_vTargetPosition;

	// If the node near the NPC or the target is outside the navmesh, return false
	FVector vStartPosition = m_pOwner->GetNPC()->GetActorLocation();
	NavNodeRef closestNodeToNPC = GetClosestNodeToPosition(vStartPosition);
	NavNodeRef closestNodeToTarget = GetClosestNodeToPosition(p_vTargetPosition);

	if (closestNodeToNPC == -1 || closestNodeToTarget == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Path false"));
		return false;
	}

	PathFinder* pPathFinder = new PathFinder(m_pRecastNavMesh, m_pNavigationSystem);
	TArray<NavNodeRef> aPathNodes = pPathFinder->AStar(m_pOwner->GetWorld(), closestNodeToNPC, closestNodeToTarget);
	
	if (!aPathNodes.IsEmpty())
	{
		p_aPath = pPathFinder->FunnelAlgorithm(m_pOwner->GetWorld(), aPathNodes, vStartPosition, p_vTargetPosition);
		
		if (!p_aPath.IsEmpty())
		{
			for (int i = 0; i < p_aPath.Num(); i++)
			{
				DrawDebugLine(m_pOwner->GetWorld(), p_aPath[i].GetSourcePosition(), p_aPath[i].GetDestinationPosition(), FColor::Purple, false, -1.0f, 0, 5.0f);
			}
		}
	}

	//GEngine->AddOnScreenDebugMessage(0, -1.0f, FColor::Purple, FString::Printf(TEXT("Heuristic cost: %f"), fCost));

	return false;
}