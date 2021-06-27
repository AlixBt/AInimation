#include "WanderAndLookForPreyState.h"
#include "AIIrex.h"

WanderAndLookForPreyState* WanderAndLookForPreyState::Instance()
{
	static WanderAndLookForPreyState instance;

	return &instance;
}

void WanderAndLookForPreyState::EnterState(AAIIrex* p_pOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("WanderAndLookForPreyState::EnterState"));
}

void WanderAndLookForPreyState::ExecuteState(AAIIrex* p_pOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("WanderAndLookForPreyState::ExecuteState"));

	if (!p_pOwner->GetPreyIsFound())
	{
		p_pOwner->FollowPath();
	}
}

void WanderAndLookForPreyState::ExitState(AAIIrex* p_pOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("WanderAndLookForPreyState::ExitState"));
}
