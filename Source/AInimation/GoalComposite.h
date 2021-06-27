#pragma once

#include "CoreMinimal.h"
#include "Goal.h"

/**
 * 
 */
template<class T>
class AINIMATION_API GoalComposite : public Goal<T>
{
	TArray<Goal*> m_aSubgoals;
public:
	GoalComposite();
	~GoalComposite();

	// Virtual functions override
	void Activate() override;
	int Process() override;
	void Terminate() override;
	void AddSubgoal(Goal<T>* p_goal) override;

	// Composite functions
	EStatus ProcessSubgoals();
	void RemoveAllSubgoals();
};
