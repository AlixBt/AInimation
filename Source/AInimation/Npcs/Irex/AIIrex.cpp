#include "AIIrex.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Navigation/PathFollowingComponent.h"
#include "../../Goap/GoalThink.h"
#include "../../AI/SteeringBehaviors.h"
#include "GameFramework/CharacterMovementComponent.h"

AAIIrex::AAIIrex(FObjectInitializer const& p_objectInitializer) :
	m_brain(new GoalThink(this)),
	m_npcCharacter(nullptr),
	m_npcAnimInstance(nullptr),
	m_pNavigationSystem(nullptr),
	m_path(nullptr)
{
	// Initialization
	m_pPathPlanner = new PathPlanner(this);
	m_steeringBehaviors = new SteeringBehaviors(this);
}

void AAIIrex::BeginPlay()
{
	Super::BeginPlay();

	// Catch the path for later use
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AACPath::StaticClass(), OutActors);

	if (OutActors.Num() > 0)
	{
		m_path = Cast<AACPath>(OutActors[0]);
		UE_LOG(LogTemp, Warning, TEXT("AAIIrex::BeginPlay - Path catch"));
	}

	// We catch the navigation system for the path planner
	m_pNavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (m_pNavigationSystem)
	{
		m_pPathPlanner->InitializeNavMesh(m_pNavigationSystem);
	}
}

void AAIIrex::OnPossess(APawn* const p_pawn)
{
	Super::OnPossess(p_pawn);

	// We use p_pawn to cast to the corresponding class
	if (p_pawn->IsA(ACIrex::StaticClass()))
	{
		m_npcCharacter = Cast<ACIrex>(p_pawn);

		if (m_npcCharacter)
		{
			m_npcAnimInstance = Cast<UAIrex>(m_npcCharacter->GetMesh()->GetAnimInstance());
		}
	}
}

void AAIIrex::Tick(float p_deltaTime)
{
	Super::Tick(p_deltaTime);

	// Main tick loop
	if (m_npcCharacter && m_npcAnimInstance)
	{

		if (m_brain != nullptr)
		{
			updateSteeringBehaviors();

			m_brain->arbitrate();
			EStatus status = m_brain->Process();
			GEngine->AddOnScreenDebugMessage(0, -1.0f, FColor::Red, FString::Printf(TEXT("Index: %i"), m_path->GetPathIndex()));
			GEngine->AddOnScreenDebugMessage(1, -1.0f, FColor::Yellow, FString::Printf(TEXT("Is following a path: %s"), m_bIsFollowingPath ? TEXT("true") : TEXT("false")));
		}
	}
}

ACIrex* AAIIrex::GetNPC() const
{
	return m_npcCharacter;
}

GoalThink* AAIIrex::getBrain() const
{
	return m_brain;
}

PathPlanner* AAIIrex::getPathPlanner() const
{
	return m_pPathPlanner;
}

UNavigationSystemV1* AAIIrex::getNavigationSystem() const
{
	return m_pNavigationSystem;
}

AACPath* AAIIrex::getPath() const
{
	return m_path;
}

bool AAIIrex::getIsFollowingPath() const
{
	return m_bIsFollowingPath;
}

SteeringBehaviors* AAIIrex::getSteeringBehaviors() const
{
	return m_steeringBehaviors;
}

FVector AAIIrex::getVelocity() const
{
	return m_velocity;
}

FVector AAIIrex::getForwardVector() const
{
	return m_forwardVector;
}

FVector AAIIrex::getRightVector() const
{
	return m_rightVector;
}

float AAIIrex::getMass() const
{
	return m_mass;
}

float AAIIrex::getMaxSpeed() const
{
	return m_maxSpeed;
}

float AAIIrex::getMaxForce() const
{
	return m_maxForce;
}

float AAIIrex::getMaxTurnRate() const
{
	return m_maxTurnRate;
}

void AAIIrex::setIsFollowingPath(bool t_bIsFollowingPath)
{
	m_bIsFollowingPath = t_bIsFollowingPath;
}

void AAIIrex::updateSteeringBehaviors()
{
	/* FVector steeringForce = m_steeringBehaviors->calculate(); */
	/* FVector acceleration = steeringForce / m_mass;
	m_velocity += acceleration * GetWorld()->DeltaTimeSeconds;

	if (m_velocity.Size() > m_maxSpeed)
	{
		UKismetMathLibrary::ClampVectorSize(m_velocity, 0.0f, m_maxSpeed);
	} */

	FVector npcPosition = FVector(m_npcCharacter->GetActorLocation().X,
	                              m_npcCharacter->GetActorLocation().Y,
								  0.0f);

    FVector targetPosition = FVector(m_steeringBehaviors->getTargetPosition().X,
	                                 m_steeringBehaviors->getTargetPosition().Y,
								     0.0f);

	FRotator nextRotation = UKismetMathLibrary::FindLookAtRotation(
		                    npcPosition,
							targetPosition);

	m_npcCharacter->SetActorRotation(UKismetMathLibrary::RInterpTo(
		                             m_npcCharacter->GetActorRotation(), nextRotation, 
									 GetWorld()->DeltaTimeSeconds, 
									 0.8f));

	m_npcCharacter->AddActorWorldOffset(m_npcCharacter->GetActorForwardVector() * 
	                                    m_maxSpeed * GetWorld()->DeltaTimeSeconds);

	if (m_velocity.Size() > 0.00000001f)
	{
		m_forwardVector = m_velocity.GetSafeNormal(1.0f);
		m_rightVector = m_forwardVector.RightVector;
	}

	m_maxSpeed = m_npcCharacter->GetCharacterMovement()->MaxWalkSpeed;
}
