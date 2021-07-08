#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CIrex.h"
#include "AIrex.generated.h"

/**
 * 
 */
UCLASS()
class AINIMATION_API UAIrex : public UAnimInstance
{
	GENERATED_BODY()
	
	// Utility
	ACIrex* m_npcCharacter;

public:
	// Constructor
	UAIrex();

	// Inherited UAnimInstance functions
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float p_deltaSeconds) override;

	// Procedural animation
	FVector ForwardKinematics() const;
};
