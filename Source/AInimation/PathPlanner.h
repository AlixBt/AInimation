#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

/**
 * 
 */
template<class T>
class AINIMATION_API PathPlanner
{
	T* m_pOwner;
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
			m_recastNavMesh = Cast<ARecastNavMesh>(&p_pNavigationSystem->GetMainNavDataChecked());
		}

		if (m_recastNavMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("PathPlanner::InitializeNavMesh() - Recast nav mesh found"));
		}
	}

	int GetClosestNodeToPosition(FVector p_vPosition, TArray<FVector>& p_outVerts) const
	{
		// We catch the closest node on the navmesh, to our position
		NavNodeRef closestNode = m_recastNavMesh->FindNearestPoly(p_vPosition, FVector(50.0f, 50.0f, 50.0f));

		if (closestNode != INVALID_NAVNODEREF)
		{
			TArray<FVector> outVerts;
			m_recastNavMesh->GetPolyVerts(closestNode, p_outVerts);

			/*for (int i = 0; i < outVerts.Num(); i++)
			{
				if (i == outVerts.Num() - 1)
				{
					DrawDebugLine(GEngine->GetWorld(), outVerts[i] + FVector(0.0f, 0.0f, 50.0f), outVerts[0] + FVector(0.0f, 0.0f, 50.0f), FColor::Red, false, 0.0f, 0, 10.0f);
					UE_LOG(LogTemp, Warning, TEXT("Test"));
				}
				else
				{
					DrawDebugLine(GEngine->GetWorld(), outVerts[i] + FVector(0.0f, 0.0f, 50.0f), outVerts[i + 1] + FVector(0.0f, 0.0f, 50.0f), FColor::Red, false, 0.0f, 0, 10.0f);
				}
			}*/

			return closestNode;
		}

		return -1;
	}
};
