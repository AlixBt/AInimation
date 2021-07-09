#pragma once

#include "CoreMinimal.h"

class AAIIrex;

/**
 * 
 */
class AINIMATION_API SteeringBehaviors
{
	AAIIrex* m_owner;
	FVector m_targetPosition;

	// Steering behaviors switchs
	bool m_bSeekIsOn { false };
	bool m_bFleeIsOn { false };
	bool m_bArriveIsOn { false };

public:
	SteeringBehaviors(AAIIrex* t_owner);
	~SteeringBehaviors();

	// Steering behaviors
	FVector seek(FVector t_targetPosition) const;
	FVector flee(FVector t_targetPosition) const;
	FVector arrive(FVector t_targetPosition) const;

	FVector calculate() const;

	FVector getTargetPosition() const;
	void setTargetPosition(FVector t_targetPosition);

	void seekOn();
	void fleeOn();
	void arriveOn();

	void seekOff();
	void fleeOff();
	void arriveOff();
};
