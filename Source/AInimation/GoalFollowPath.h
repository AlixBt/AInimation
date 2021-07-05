#pragma once

#include "CoreMinimal.h"
#include "GoalComposite.h"
#include "PathEdge.h"

/**
 * 
 */
class AINIMATION_API GoalFollowPath : public GoalComposite
{
	TArray<PathEdge> m_aPath;

public:
	GoalFollowPath(AAIIrex* p_pOwner, TArray<PathEdge> p_aPath);
	~GoalFollowPath();

	// Override function
	virtual void Activate() override;
	virtual EStatus Process() override;
	virtual void Terminate() override;
};
