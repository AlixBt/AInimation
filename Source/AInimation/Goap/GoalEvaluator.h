#pragma once

#include "CoreMinimal.h"

class AAIIrex;

/**
 * 
 */
class AINIMATION_API GoalEvaluator
{

public:
	GoalEvaluator();
	virtual ~GoalEvaluator();

	// Pure virtual functions
	virtual float calculateDesirability(AAIIrex* t_owner) = 0;
	virtual void setGoal(AAIIrex* t_owner) = 0;
};
