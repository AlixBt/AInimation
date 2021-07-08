#pragma once

#include "CoreMinimal.h"
#include "GoalComposite.h"
#include "../Pathfinding/PathEdge.h"

/**
 * 
 */
class AINIMATION_API GoalFollowPath : public GoalComposite
{
	TArray<PathEdge> m_aPath;

public:
	GoalFollowPath(AAIIrex* p_pOwner, TArray<PathEdge> p_aPath);
	virtual ~GoalFollowPath();

	// Override function
	virtual void Activate() override;
	virtual EStatus Process() override;
	virtual void Terminate() override;
};
