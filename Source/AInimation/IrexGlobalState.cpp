#include "IrexGlobalState.h"
#include "AIIrex.h"

IrexGlobalState* IrexGlobalState::Instance()
{
	static IrexGlobalState instance;

	return &instance;
}

void IrexGlobalState::EnterState(AAIIrex* p_pOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("IrexGlobalState::EnterState"));
}

void IrexGlobalState::ExecuteState(AAIIrex* p_pOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("IrexGlobalState::ExecuteState"));
}

void IrexGlobalState::ExitState(AAIIrex* p_pOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("IrexGlobalState::ExitState"));
}
