#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EStatus : uint8
{
	ES_Active UMETA(DisplayName = "Active"),
	ES_Inactive UMETA(DisplayName = "Inactive"),
	ES_Completed UMETA(DisplayName = "Completed"),
	ES_Failed UMETA(DisplayName = "Failed")
};

template<class T>
class AINIMATION_API Goal
{
protected:
	T* m_pOwner;
	EStatus m_eStatus;
	int m_iType;

public:
	Goal(T* p_pOwner);
	Goal(T* p_pOwner ,int p_iType);
	~Goal();

	// Pure virtual functions
	virtual void Activate() = 0;
	virtual int Process() = 0;
	virtual void Terminate() = 0;
	virtual void AddSubgoal(Goal<T>* p_goal) = 0;

	// Getters
	bool IsActive() const;
	bool IsInactive() const;
	bool IsCompleted() const;
	bool HasFailed() const;
	int GetType() const;
};
