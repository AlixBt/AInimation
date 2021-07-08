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
}

EStatus GoalComposite::Process()
{
	return EStatus::ES_Active;
}

void GoalComposite::Terminate()
{
}

void GoalComposite::AddSubgoal(Goal* p_goal)
{
	m_aSubgoals.Insert(p_goal, 0);
}

EStatus GoalComposite::ProcessSubgoals()
{
	while (!m_aSubgoals.IsEmpty() && m_aSubgoals[0]->IsCompleted() 
		   || m_aSubgoals[0]->HasFailed())
	{
		m_aSubgoals[0]->Terminate();
		delete m_aSubgoals[0];
		m_aSubgoals.RemoveAt(0, 1, true);

		if (m_aSubgoals.IsEmpty())
		{
			break;
		}
	}

	if (!m_aSubgoals.IsEmpty())
	{
		EStatus subgoalsStatus = m_aSubgoals[0]->Process();

		if (subgoalsStatus == EStatus::ES_Completed && m_aSubgoals.Num() > 1)
		{
			return EStatus::ES_Active;
		}

		return subgoalsStatus;
	}

	else
	{
		return EStatus::ES_Completed;
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
