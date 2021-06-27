


#include "Goal.h"

template<class T>
Goal<T>::Goal(T* p_pOwner):
	m_pOwner(p_pOwner),
	m_eStatus(EStatus::ES_Inactive),
	m_iType(-1)
{
}

template<class T>
Goal<T>::Goal(T* p_pOwner, int p_iType) :
	m_pOwner(p_pOwner),
	m_eStatus(EStatus::ES_Inactive),
	m_iType(p_iType)
{
}

template<class T>
Goal<T>::~Goal()
{
}

template<class T>
bool Goal<T>::IsActive() const
{
	if (m_eStatus == EStatus::ES_Active)
		return true;

	return false;
}

template<class T>
bool Goal<T>::IsInactive() const
{
	if (m_eStatus == EStatus::ES_Inactive)
		return true;

	return false;
}

template<class T>
bool Goal<T>::IsCompleted() const
{
	if (m_eStatus == EStatus::ES_Completed)
		return true;

	return false;
}

template<class T>
bool Goal<T>::HasFailed() const
{
	if (m_eStatus == EStatus::ES_Failed)
		return true;

	return false;
}

template<class T>
int Goal<T>::GetType() const
{
	return m_iType;
}
