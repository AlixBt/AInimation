#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CIrex.h"
#include "AIrex.h"
#include "../../ACPath.h"
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
	ACIrex* m_npcCharacter;
	UAIrex* m_npcAnimInstance;
	UNavigationSystemV1* m_pNavigationSystem;

	// PathFinding
	PathPlanner* m_pPathPlanner;
	AACPath* m_path;

	// Path following
	FVector m_targetPosition {FVector::ZeroVector};

public:
	// Constructor
	AAIIrex(FObjectInitializer const& p_objectInitializer = FObjectInitializer::Get());

	// Inherited AAIController functions
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* const p_pawn) override;
	virtual void Tick(float p_deltaTime) override;

	// Getters
	bool GetPreyIsFound() const;
	ACIrex* GetNPC() const;
	GoalThink* getBrain() const;
	PathPlanner* getPathPlanner() const;
	UNavigationSystemV1* getNavigationSystem() const;
	AACPath* getPath() const;
	bool getIsFollowingPath() const;
	FVector getTargetPosition() const;

	// Setters
	void setIsFollowingPath(bool t_bIsFollowingPath);
	void setTargetPosition(FVector t_targetPosition);

	void setMovementBehaviors(float t_deltaTime);
};
