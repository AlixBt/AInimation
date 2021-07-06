#pragma once

#include "CoreMinimal.h"
#include "../Npcs/Irex/AIIrex.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EStatus : uint8
{
	ES_Active = 0 UMETA(DisplayName = "Active"),
	ES_Inactive = 1 UMETA(DisplayName = "Inactive"),
	ES_Completed = 2 UMETA(DisplayName = "Completed"),
	ES_Failed = 3 UMETA(DisplayName = "Failed")
};

class AINIMATION_API Goal
{
protected:
	AAIIrex* m_pOwner;
	EStatus m_eStatus;
	int m_iType;

public:
	Goal(AAIIrex* p_pOwner);
	Goal(AAIIrex* p_pOwner ,int p_iType);
	virtual ~Goal();

	// Pure virtual functions
	virtual void Activate() = 0;
	virtual EStatus Process() = 0;
	virtual void Terminate() = 0;
	virtual void AddSubgoal(Goal* p_goal) {}

	// Getters
	bool IsActive() const;
	bool IsInactive() const;
	bool IsCompleted() const;
	bool HasFailed() const;
	int GetType() const;
};
