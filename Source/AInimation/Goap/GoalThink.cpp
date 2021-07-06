#include "GoalThink.h"
#include "GoalFollowPath.h"
#include "ExploreGoalEvaluator.h"
#include "NavigationSystem.h"
#include "../Pathfinding/PathPlanner.h"
#include "../Pathfinding/PathEdge.h"
#include "../ACPath.h"

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
	m_eStatus = EStatus::ES_Active;
}

EStatus GoalThink::Process()
{
	if (m_eStatus != EStatus::ES_Active)
		Activate();

	m_eStatus = ProcessSubgoals();

	return m_eStatus;
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
}

void GoalThink::addGoalExplore()
{
	// We choose an arbitrary position on the navmesh
	AACPath* m_path = m_pOwner->getPath();
	FVector point = m_path->GetPathPoint();

	TArray<PathEdge> pathfind;
	m_pOwner->getPathPlanner()->CreatePathToPosition(point, pathfind);

	AddSubgoal(new GoalFollowPath(m_pOwner, pathfind));
	UE_LOG(LogTemp, Warning, TEXT("GoalThink::addGoalExplore() - follow a path to an arbitrary point"));
}
