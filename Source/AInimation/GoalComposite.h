#pragma once

#include "CoreMinimal.h"
#include "Goal.h"

/**
 * 
 */
class AINIMATION_API GoalComposite : public Goal
{
	TArray<Goal*> m_aSubgoals;
public:
	GoalComposite(AAIIrex* p_pOwner);
	~GoalComposite();

	// Virtual functions override
	virtual void Activate() override;
	virtual int Process() override;
	virtual void Terminate() override;
	void AddSubgoal(Goal* p_goal);

	// Composite functions
	int ProcessSubgoals();
	void RemoveAllSubgoals();
};
