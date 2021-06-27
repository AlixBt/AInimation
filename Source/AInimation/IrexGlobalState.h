

#pragma once

#include "CoreMinimal.h"
#include "State.h"

/**
 * 
 */
class AINIMATION_API IrexGlobalState : public State<class AAIIrex>
{
	// Singleton pattern
	IrexGlobalState() {}
public:
	// Singleton pattern
	static IrexGlobalState* Instance();

	// Override State functions
	virtual void EnterState(class AAIIrex* p_pOwner) override;
	virtual void ExecuteState(class AAIIrex* p_pOwner) override;
	virtual void ExitState(class AAIIrex* p_pOwner) override;

	~IrexGlobalState() {}
};
