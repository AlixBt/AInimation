#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CIrex.h"
#include "AIrex.h"
#include "StateMachine.h"
#include "ACPath.h"
#include "NavigationSystem.h"
#include "PathPlanner.h"
#include "AIIrex.generated.h"

/**
 * 
 */
UCLASS()
class AINIMATION_API AAIIrex : public AAIController
{
	GENERATED_BODY()

	// Finite state machine
	StateMachine<AAIIrex>* m_pStateMachine;

	bool m_bPreyIsFound;

	// Utility
	ACIrex* m_npcCharacter;
	UAIrex* m_npcAnimInstance;
	UNavigationSystemV1* m_pNavigationSystem;

	// PathFinding
	PathPlanner<AAIIrex>* m_pPathPlanner;
	AACPath* m_path;

public:
	// Constructor
	AAIIrex(FObjectInitializer const& p_objectInitializer = FObjectInitializer::Get());

	// Inherited AAIController functions
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* const p_pawn) override;
	virtual void Tick(float p_deltaTime) override;

	// Getters
	bool GetPreyIsFound() const;

	// Pathfinding
	void FollowPath();
};
