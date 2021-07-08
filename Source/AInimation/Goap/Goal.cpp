#include "Goal.h"

Goal::Goal(AAIIrex* p_pOwner):
	m_pOwner(p_pOwner),
	m_eStatus(EStatus::ES_Inactive),
	m_iType(-1)
{
}

Goal::Goal(AAIIrex* p_pOwner, int p_iType) :
	m_pOwner(p_pOwner),
	m_eStatus(EStatus::ES_Inactive),
	m_iType(p_iType)
{
}

Goal::~Goal()
{
}

bool Goal::IsActive() const
{
	if (m_eStatus == EStatus::ES_Active)
		return true;

	return false;
}

bool Goal::IsInactive() const
{
	if (m_eStatus == EStatus::ES_Inactive)
		return true;

	return false;
}

bool Goal::IsCompleted() const
{
	if (m_eStatus == EStatus::ES_Completed)
		return true;

	return false;
}

bool Goal::HasFailed() const
{
	if (m_eStatus == EStatus::ES_Failed)
		return true;

	return false;
}

int Goal::GetType() const
{
	return m_iType;
}
