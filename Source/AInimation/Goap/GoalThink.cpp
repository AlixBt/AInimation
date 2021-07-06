#include "GoalThink.h"
#include "ExploreGoalEvaluator.h"

GoalThink::GoalThink(AAIIrex* p_pOwner) :
	GoalComposite(p_pOwner)
{
	m_goalsEvaluator.Add(new ExploreGoalEvaluator());
}

GoalThink::~GoalThink()
{
}

void GoalThink::Activate()
{
}

EStatus GoalThink::Process()
{
	return EStatus();
}

void GoalThink::Terminate()
{
}

void GoalThink::arbitrate()
{
	GoalEvaluator* bestEvaluator = nullptr;
	double bestDesirability = 0;

	for (int i = 0; i < m_goalsEvaluator.Num(); ++i)
	{
		double desirability = m_goalsEvaluator[i]->calculateDesirability(m_pOwner);

		if (desirability >= bestDesirability)
		{
			bestDesirability = desirability;
			bestEvaluator = m_goalsEvaluator[i];
		}
	}

	if (bestEvaluator != nullptr)
	{
		bestEvaluator->setGoal(m_pOwner);
	}
	UE_LOG(LogTemp, Warning, TEXT("GoalThink::arbitrate() - calculate best goal cost"));
}

void GoalThink::addGoalExplore()
{
	UE_LOG(LogTemp, Warning, TEXT("GoalThink::addGoalExplore() - follow a path to an arbitrary point"));
}
