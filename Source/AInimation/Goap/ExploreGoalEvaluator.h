#pragma once

#include "CoreMinimal.h"
#include "GoalEvaluator.h"

class AAIIrex;

/**
 * 
 */
class AINIMATION_API ExploreGoalEvaluator : public GoalEvaluator
{
public:
	ExploreGoalEvaluator();
	virtual ~ExploreGoalEvaluator();

	// Override pure virtual functions
	virtual float calculateDesirability(AAIIrex* t_owner) override;
	virtual void setGoal(AAIIrex* t_owner) override;
};
