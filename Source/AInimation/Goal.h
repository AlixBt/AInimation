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
	Goal();
	Goal(int p_iType);
	~Goal();

	// Pure virtual functions
	virtual void Activate() { UE_LOG(LogTemp, Warning, TEXT("Goal::Activate() - Goal is active")); };
	virtual int Process() { UE_LOG(LogTemp, Warning, TEXT("Goal::Process() - Goal is processing")); };
	virtual void Terminate() { UE_LOG(LogTemp, Warning, TEXT("Goal::Terminate() - Goal is terminated")); };
	virtual void AddSubgoal(Goal<T>* p_goal) { UE_LOG(LogTemp, Warning, TEXT("Goal::AddSubgoals() - Goal has been added")); };

	// Getters
	bool IsActive() const;
	bool IsInactive() const;
	bool IsCompleted() const;
	bool HasFailed() const;
	int GetType() const;
};
