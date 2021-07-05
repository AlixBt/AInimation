#include "GoalFollowPath.h"
#include "GoalTraversePath.h"

GoalFollowPath::GoalFollowPath(AAIIrex* p_pOwner, TArray<PathEdge> p_aPath) :
	GoalComposite(p_pOwner),
	m_aPath(p_aPath)
{
}

GoalFollowPath::~GoalFollowPath()
{
}

void GoalFollowPath::Activate()
{
	m_eStatus = EStatus::ES_Active;

	// We get the next edge and remove it from the array
	PathEdge nextEdge = m_aPath[0];
	m_aPath.RemoveAt(0, 1, true);

	// We add the appropriate goals, based on the behavior flag of the path edge
	switch (nextEdge.GetBehaviorType())
	{
	case EBehaviorType::EBT_Normal:
	{
		AddSubgoal(new GoalTraversePath(m_pOwner, nextEdge, m_aPath.IsEmpty()));
		break;
	}
	default:
	{
		UE_LOG(LogTemp, Warning, TEXT("GoalFollowPath::Activate() - Unrecognized behavior type"));
		break;
	}
	}

	UE_LOG(LogTemp, Warning, TEXT("GoalFollowPath"));
}

EStatus GoalFollowPath::Process()
{
	if (m_eStatus != EStatus::ES_Active)
		Activate();

	// If there is no subgoals to check and the path has still edge to traverse,
	// call the Activate function to grab the next edge
	m_eStatus = ProcessSubgoals();
	if (m_eStatus == EStatus::ES_Completed && !m_aPath.IsEmpty())
	{
		Activate();
	}

	UE_LOG(LogTemp, Warning, TEXT("GoalFollowPath::Process() - Goal is processing"));

	return m_eStatus;
}

void GoalFollowPath::Terminate()
{
}
