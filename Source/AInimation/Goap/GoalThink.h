#pragma once

#include "CoreMinimal.h"
#include "GoalComposite.h"

/**
 * 
 */
class AINIMATION_API GoalThink : public GoalComposite
{
public:
	GoalThink(AAIIrex* p_pOwner);
	virtual ~GoalThink();

	virtual void Activate() override;
	virtual EStatus Process() override;
	virtual void Terminate() override;
};
