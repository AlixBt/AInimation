#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"

/**
 * 
 */
class AINIMATION_API PathPlanner
{
	class AAIIrex* m_pOwner;
	class ARecastNavMesh* m_pRecastNavMesh;
	class UNavigationSystemV1* m_pNavigationSystem;
	FVector m_vDestinationToReach;

public:
	PathPlanner(AAIIrex* p_pOwner);
	~PathPlanner();

	// Path planning functions
	void InitializeNavMesh(UNavigationSystemV1* p_pNavigationSystem);
	NavNodeRef GetClosestNodeToPosition(FVector p_vPosition) const;
	bool CreatePathToPosition(FVector p_vTargetPosition, TArray<FVector>& p_aPath);
		
};
