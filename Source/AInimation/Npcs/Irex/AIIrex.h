#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Irex.h"
#include "AIrex.h"
#include "../../Utility/CartTarget.h"
#include "NavigationSystem.h"
#include "../../Pathfinding/PathPlanner.h"
#include "AIIrex.generated.h"

class GoalThink;

/**
 * 
 */
UCLASS()
class AINIMATION_API AAIIrex : public AAIController
{
	GENERATED_BODY()

	// G.O.A.P system
	GoalThink* m_brain;
	bool m_bIsFollowingPath{ false };

	// Utility
	AIrex* m_npcCharacter;
	UAIrex* m_npcAnimInstance;
	UNavigationSystemV1* m_pNavigationSystem;

	// PathFinding
	PathPlanner* m_pPathPlanner;
	ACartTarget* m_target;

	// Path following
	FVector m_targetPosition {FVector::ZeroVector};
	float m_turnRate {120.0f};

public:
	// Constructor
	AAIIrex(FObjectInitializer const& p_objectInitializer = FObjectInitializer::Get());

	// Inherited AAIController functions
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* const p_pawn) override;
	virtual void Tick(float p_deltaTime) override;

	// Getters
	bool GetPreyIsFound() const;
	AIrex* GetNPC() const;
	GoalThink* getBrain() const;
	PathPlanner* getPathPlanner() const;
	UNavigationSystemV1* getNavigationSystem() const;
	ACartTarget* getPath() const;
	bool getIsFollowingPath() const;
	FVector getTargetPosition() const;
	float getTurnRate() const;

	// Setters
	void setIsFollowingPath(bool t_bIsFollowingPath);
	void setTargetPosition(FVector t_targetPosition);
	void setTurnRate(float t_turnRate);

	void setMovementBehaviors(float t_deltaTime);
};
