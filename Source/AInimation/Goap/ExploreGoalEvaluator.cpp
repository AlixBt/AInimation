#include "ExploreGoalEvaluator.h"
#include "../Npcs/Irex/AIIrex.h"
#include "GoalThink.h"

ExploreGoalEvaluator::ExploreGoalEvaluator() :
	GoalEvaluator()
{
}

ExploreGoalEvaluator::~ExploreGoalEvaluator()
{
}

float ExploreGoalEvaluator::calculateDesirability(AAIIrex* t_owner)
{
	return 0.05f;
}

void ExploreGoalEvaluator::setGoal(AAIIrex* t_owner)
{
	t_owner->getBrain()->addGoalExplore();
}
