#include "GoalComposite.h"

GoalComposite::GoalComposite(AAIIrex* p_pOwner) :
	Goal( p_pOwner, 1)
{
}

GoalComposite::~GoalComposite()
{
}

void GoalComposite::Activate()
{
	m_eStatus = EStatus::ES_Active;

	UE_LOG(LogTemp, Warning, TEXT("GoalComposite::Activate() - Goal is active"));
}

int GoalComposite::Process()
{
	if (m_eStatus != EStatus::ES_Active)
		Activate();

	UE_LOG(LogTemp, Warning, TEXT("GoalComposite::Process() - Goal is processing"));

	return (int)m_eStatus;
}

void GoalComposite::Terminate()
{
	UE_LOG(LogTemp, Warning, TEXT("GoalComposite::Terminate() - Goal is terminated"));
}

void GoalComposite::AddSubgoal(Goal* p_goal)
{
	m_aSubgoals.Insert(p_goal, 0);

	UE_LOG(LogTemp, Warning, TEXT("GoalComposite::AddSubgoals() - Goal has been added"));
}

int GoalComposite::ProcessSubgoals()
{
	while (m_aSubgoals.Num() > 0 && m_aSubgoals[0]->IsCompleted() || m_aSubgoals[0]->HasFailed())
	{
		m_aSubgoals[0]->Terminate();
		m_aSubgoals.RemoveAt(0, 1, true);
	}

	if (m_aSubgoals.Num() > 0)
	{
		uint8 subgoalsStatus = m_aSubgoals[0]->Process();

		if (subgoalsStatus == (int)EStatus::ES_Completed && m_aSubgoals.Num() > 1)
		{
			return (int)EStatus::ES_Active;
		}

		return subgoalsStatus;
	}
	else
	{
		return (int)EStatus::ES_Completed;
	}
}

void GoalComposite::RemoveAllSubgoals()
{
	for (int i = 0; i < m_aSubgoals.Num(); i++)
	{
		m_aSubgoals[i]->Terminate();
		m_aSubgoals.RemoveAt(i, 1, true);
	}

	m_aSubgoals.Empty();
}
