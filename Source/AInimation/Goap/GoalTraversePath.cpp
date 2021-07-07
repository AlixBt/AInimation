#include "GoalTraversePath.h"

bool GoalTraversePath::NPCIsStuck() const
{
	if (m_vLastNPCPosition.Equals(m_pOwner->GetNPC()->GetActorLocation(), 1.0f) 
		&& !m_vLastNPCPosition.Equals(m_pathEdge.GetDestinationPosition(), 50.0f))
		return true;

	return false;
}

GoalTraversePath::GoalTraversePath(AAIIrex* p_pOwner, PathEdge p_pathEdge, bool p_bLastEdge) :
	Goal(p_pOwner),
	m_pathEdge(p_pathEdge),
	m_bLastEdgeInPath(p_bLastEdge),
	m_vLastNPCPosition(FVector::ZeroVector)
{
}

GoalTraversePath::~GoalTraversePath()
{
}

void GoalTraversePath::Activate()
{
	m_eStatus = EStatus::ES_Active;

	switch (m_pathEdge.GetBehaviorType())
	{
		case EBehaviorType::EBT_Climb:
		{
			break;
		}

		case EBehaviorType::EBT_Swim:
		{
			break;
		}

		default:
		{
			break;
		}
	}

	/* TODO
	 * 
	 * Set the steering target 
	 * Set the steering behavior
	 * 
	 */

	// Set the appropriate steering behavior. If it is the last edge,
	// the NPC should arrive at the position, else it should seek
	if (m_bLastEdgeInPath)
	{
		m_pOwner->MoveToLocation(m_pathEdge.GetDestinationPosition(), 100.0f);
		UE_LOG(LogTemp, Warning, TEXT("Last edge: move to location"));
	}
	else
	{
		m_pOwner->MoveToLocation(m_pathEdge.GetDestinationPosition(), 100.0f);
		UE_LOG(LogTemp, Warning, TEXT("Move to location"));
	}
}

EStatus GoalTraversePath::Process()
{
	if (m_eStatus != EStatus::ES_Active)
		Activate();

	// If the NPC is stuck, return failure
	if (NPCIsStuck())
	{
		m_eStatus = EStatus::ES_Failed;
		UE_LOG(LogTemp, Warning, TEXT("Npc is stuck"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Npc is moving"));
		// We check if the NPC is at destination with a radius of 50.0f
		if (m_bLastEdgeInPath)
		{
			if (m_pOwner->GetNPC()->GetActorLocation().Equals(m_pathEdge.GetDestinationPosition(), 300.0f))
			{
				m_pOwner->getPath()->IncrementPathIndex();
				m_eStatus = EStatus::ES_Completed;
				m_pOwner->setIsFollowingPath(false);
				UE_LOG(LogTemp, Warning, TEXT("Path completed"));
			}
		}
	}

	// We record the last NPC position to check if he is stuck on the edge
	m_vLastNPCPosition = m_pOwner->GetNPC()->GetActorLocation();

	return m_eStatus;
}

void GoalTraversePath::Terminate()
{
	/* TODO
	 *
	 * Set the steering behavior off
	 * Set the NPC's speed
	 *
	 */
}
