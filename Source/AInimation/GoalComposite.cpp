#include "GoalComposite.h"

template<class T>
GoalComposite<T>::GoalComposite() : 
	Goal<T>(1)
{
}

template<class T>
GoalComposite<T>::~GoalComposite()
{
}

template<class T>
void GoalComposite<T>::Activate()
{
	m_eStatus = EStatus::ES_Active;
}

template<class T>
int GoalComposite<T>::Process()
{
	if (m_eStatus != EStatus::ES_Active)
		Activate();

	return m_eStatus;
}

template<class T>
void GoalComposite<T>::Terminate()
{
	UE_LOG(LogTemp, Warning, TEXT("GoalComposite::Terminate() - Goal is terminated"));
}

template<class T>
void GoalComposite<T>::AddSubgoal(Goal<T>* p_goal)
{
	m_aSubgoals.Insert(p_goal, 0);
}

template<class T>
EStatus GoalComposite<T>::ProcessSubgoals()
{
	while (m_aSubgoal.Num() > 0 && m_aSubgoal[0]->IsCompleted() || m_aSubgoal[0]->HasFailed())
	{
		m_aSubgoals[0]->Terminate();
		m_aSubgoals.RemoveAt(0, 1, true);
	}

	if (m_aSubgoal.Num() > 0)
	{
		int subgoalsStatus = m_aSubgoals[0]->Process();

		if (subgoals == EStatus::ES_Completed && m_aSubgoals.Num() > 1)
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

template<class T>
void GoalComposite<T>::RemoveAllSubgoals()
{
	for (int i = 0; i < m_aSubgoals.Num(); i++)
	{
		m_aSubgoals[i]->Terminate();
		m_aSubgoals.RemoveAt(i, 1, true);
	}

	m_aSubgoals.Empty();
}
