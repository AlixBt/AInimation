#pragma once

#include "CoreMinimal.h"
#include "GoalComposite.h"

class GoalEvaluator;

/**
 * 
 */
class AINIMATION_API GoalThink : public GoalComposite
{
	TArray<GoalEvaluator*> m_goalsEvaluator;

public:
	GoalThink(AAIIrex* p_pOwner);
	virtual ~GoalThink();

	// Override pure virtual functions
	virtual void Activate() override;
	virtual EStatus Process() override;
	virtual void Terminate() override;

	// Goal decision making
	void arbitrate();

	// Strategy level goals
	void addGoalExplore();
};
