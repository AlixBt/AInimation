#include "GoalThink.h"
#include "GoalFollowPath.h"
#include "ExploreGoalEvaluator.h"
#include "NavigationSystem.h"
#include "../Pathfinding/PathPlanner.h"
#include "../ACPath.h"
#include "DrawDebugHelpers.h"

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

	if (!m_path.IsEmpty())
	{
		for (int i = 0; i < m_path.Num(); i++)
		{
			DrawDebugLine(m_pOwner->GetWorld(), m_path[i].GetSourcePosition(), m_path[i].GetDestinationPosition(), FColor::Purple, false, -1.0f, 0, 5.0f);
		}
	}

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
	AACPath* path = m_pOwner->getPath();
	FVector worldPoint = path->GetTransform().TransformPosition(path->GetPathPoint());
	
	if (!m_pOwner->getIsFollowingPath())
	{
		m_pOwner->getPathPlanner()->CreatePathToPosition(worldPoint, m_path);
		AddSubgoal(new GoalFollowPath(m_pOwner, m_path));
		UE_LOG(LogTemp, Warning, TEXT("Path found"));
	}
}
