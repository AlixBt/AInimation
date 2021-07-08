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
class SteeringBehaviors;

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

	// Steering behaviors
	SteeringBehaviors* m_steeringBehaviors;

	FVector m_velocity { FVector::ZeroVector };
	FVector m_forwardVector { FVector::ZeroVector };
	FVector m_rightVector { FVector::ZeroVector };

	const float m_mass { 5400.0f };
	float m_maxSpeed { 0.0f };
	const float m_maxForce{ 52920.0f };
	const float m_maxTurnRate{ 10.0f };

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

	SteeringBehaviors* getSteeringBehaviors() const;

	FVector getVelocity() const;
	FVector getForwardVector() const;
	FVector getRightVector() const;

	float getMass() const;
	float getMaxSpeed() const;
	float getMaxForce() const;
	float getMaxTurnRate() const;

	// Setters
	void setIsFollowingPath(bool t_bIsFollowingPath);
	void updateSteeringBehaviors();
};
