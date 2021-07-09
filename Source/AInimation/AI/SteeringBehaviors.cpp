#include "SteeringBehaviors.h"
#include "../Npcs/Irex/AIIrex.h"

SteeringBehaviors::SteeringBehaviors(AAIIrex* t_owner) :
	m_owner(t_owner)
{

}

SteeringBehaviors::~SteeringBehaviors()
{
}

FVector SteeringBehaviors::seek(FVector t_targetPosition) const
{
	if (m_bSeekIsOn)
	{
		FVector desiredVelocity = FVector(t_targetPosition.X - m_owner->GetNPC()->GetActorLocation().X,
										  t_targetPosition.Y - m_owner->GetNPC()->GetActorLocation().Y, 0.0f) 
		                                  * m_owner->getMaxSpeed();

		return (desiredVelocity - m_owner->getVelocity());
	}

	return FVector::ZeroVector;
}

FVector SteeringBehaviors::flee(FVector t_targetPosition) const
{
	if (m_bFleeIsOn)
	{
		return FVector();
	}

	return FVector::ZeroVector;
}

FVector SteeringBehaviors::arrive(FVector t_targetPosition) const
{
	if (m_bArriveIsOn)
	{
		return FVector();
	}

	return FVector::ZeroVector;
}

FVector SteeringBehaviors::calculate() const
{
	return seek(m_targetPosition) + flee(m_targetPosition) + arrive(m_targetPosition);
}

FVector SteeringBehaviors::getTargetPosition() const
{
	return m_targetPosition;
}

void SteeringBehaviors::setTargetPosition(FVector t_targetPosition)
{
	UE_LOG(LogTemp, Warning, TEXT("Target position parameter: %s"), *t_targetPosition.ToString());
	m_targetPosition = t_targetPosition;
	UE_LOG(LogTemp, Warning, TEXT("Target position variable: %s"), *m_targetPosition.ToString());
}

void SteeringBehaviors::seekOn()
{
	m_bSeekIsOn = true;
}

void SteeringBehaviors::fleeOn()
{
	m_bFleeIsOn = true;
}

void SteeringBehaviors::arriveOn()
{
	m_bArriveIsOn = true;
}

void SteeringBehaviors::seekOff()
{
	m_bSeekIsOn = false;
}

void SteeringBehaviors::fleeOff()
{
	m_bFleeIsOn = false;
}

void SteeringBehaviors::arriveOff()
{
	m_bArriveIsOn = false;
}
