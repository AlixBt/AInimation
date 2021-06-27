#pragma once

#include "CoreMinimal.h"
#include "State.h"

/**
 * 
 */
class AINIMATION_API WanderAndLookForPreyState : public State<class AAIIrex>
{
	// Singleton pattern
	WanderAndLookForPreyState() {}
public:
	// Singleton pattern
	static WanderAndLookForPreyState* Instance();

	// Override State functions
	virtual void EnterState(class AAIIrex* p_pOwner) override;
	virtual void ExecuteState(class AAIIrex* p_pOwner) override;
	virtual void ExitState(class AAIIrex* p_pOwner) override;

	~WanderAndLookForPreyState() {}
};
