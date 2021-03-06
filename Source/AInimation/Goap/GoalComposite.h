#pragma once

#include "CoreMinimal.h"
#include "Goal.h"

/**
 * 
 */
class AINIMATION_API GoalComposite : public Goal
{
protected:
	TArray<Goal*> m_aSubgoals;
public:
	GoalComposite(AAIIrex* p_pOwner);
	virtual ~GoalComposite();

	// Virtual functions override
	virtual void Activate() override;
	virtual EStatus Process() override;
	virtual void Terminate() override;
	void AddSubgoal(Goal* p_goal);

	// Composite functions
	EStatus ProcessSubgoals();
	void RemoveAllSubgoals();
};
