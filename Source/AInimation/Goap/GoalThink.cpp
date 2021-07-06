#include "GoalThink.h"

GoalThink::GoalThink(AAIIrex* p_pOwner) :
	GoalComposite(p_pOwner)
{
}

GoalThink::~GoalThink()
{
}

void GoalThink::Activate()
{
}

EStatus GoalThink::Process()
{
	return EStatus();
}

void GoalThink::Terminate()
{
}
