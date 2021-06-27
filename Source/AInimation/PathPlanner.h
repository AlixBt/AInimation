#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationSystem.h"

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
		// We get the nav mesh, thank to the navigation system provided by the AI controller
		if (p_pNavigationSystem)
		{
			m_recastNavMesh = Cast<ARecastNavMesh>(&p_pNavigationSystem->GetMainNavDataChecked());
		}

		if (m_recastNavMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("PathPlanner::InitializeNavMesh() - Recast nav mesh found"));
		}
	}
};
