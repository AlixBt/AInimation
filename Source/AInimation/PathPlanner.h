#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"

/**
 * 
 */
template<class T>
class AINIMATION_API PathPlanner
{
	T* m_pOwner;
	UNavigationSystemV1* m_pNavigationSystem;
	ARecastNavMesh* m_recastNavMesh;
	FVector m_vDestinationToReach;

public:
	PathPlanner(T* p_pOwner) : m_pOwner(p_pOwner)
	{}

	~PathPlanner()
	{}

	void InitializeNavMesh(UNavigationSystemV1* p_pNavigationSystem)
	{
		// We get the navmesh, thank to the navigation system provided by the AI controller
		if (p_pNavigationSystem)
		{
			m_pNavigationSystem = p_pNavigationSystem;
			m_recastNavMesh = Cast<ARecastNavMesh>(&m_pNavigationSystem->GetMainNavDataChecked());
		}

		if (m_recastNavMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("PathPlanner::InitializeNavMesh() - Recast nav mesh found"));
		}
	}

	int GetClosestNodeToPosition(FVector p_vPosition) const
	{
		// We catch the closest node on the navmesh, to our position
		NavNodeRef closestNode = m_recastNavMesh->FindNearestPoly(p_vPosition, FVector(50.0f, 50.0f, 50.0f));

		if (closestNode != INVALID_NAVNODEREF)
		{
			return closestNode;
		}

		// Position outside of the navmesh
		return -1;
	}

	bool CreatePathToPosition(UWorld* p_pWorld, FVector p_vTargetPosition, TArray<FVector>& p_aPath)
	{
		m_vDestinationToReach = p_vTargetPosition;
		FVector vStartPosition = m_pOwner->GetNPC()->GetActorLocation();
		FVector vHitLocation;

		DrawDebugBox(p_pWorld, vStartPosition, FVector(100.0f, 100.0f, 100.0f), FColor::Green, false, -1.0f, 0, 5.0f);
		DrawDebugBox(p_pWorld, p_vTargetPosition, FVector(100.0f, 100.0f, 100.0f), FColor::Red, false, -1.0f, 0, 5.0f);

		return m_pNavigationSystem->NavigationRaycast(p_pWorld, vStartPosition + FVector(0.0f, 0.0f, 150.0f), p_vTargetPosition + FVector(0.0f, 0.0f, 150.0f), vHitLocation);
	}
};
