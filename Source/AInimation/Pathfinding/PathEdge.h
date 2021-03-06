#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	EBT_Normal = 0 UMETA(DisplayName = "Normal"),
	EBT_Climb = 1 UMETA(DisplayName = "Climb"),
	EBT_Swim = 2 UMETA(DisplayName = "Swim")
};

/**
 * 
 */
class AINIMATION_API PathEdge
{
	FVector m_vSourcePosition;
	FVector m_vDestinationPosition;
	FVector m_sourceDirection;
	FVector m_targetDirection;

	EBehaviorType m_eBehavior;

public:
	PathEdge(FVector p_vSource, FVector p_vDestination, EBehaviorType p_eBehavior);
	~PathEdge();

	// Getters
	FVector GetSourcePosition() const;
	FVector GetDestinationPosition() const;
	FVector getSourceDirection() const;
	FVector getTargetDirection() const;
	EBehaviorType GetBehaviorType() const;

	// Setters
	void SetSourcePosition(FVector p_vSourcePosition);
	void SetDestinationPosition(FVector p_vDestinationPosition);
	void setSourceDirection(FVector t_sourceDirection);
	void setTargetDirection(FVector t_targetDirection);
};
