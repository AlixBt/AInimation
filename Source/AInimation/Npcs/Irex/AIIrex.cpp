#include "AIIrex.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Navigation/PathFollowingComponent.h"
#include "../../Goap/GoalThink.h"
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
	if (p_pawn->IsA(AIrex::StaticClass()))
	{
		m_npcCharacter = Cast<AIrex>(p_pawn);

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
			setMovementBehaviors(p_deltaTime);
			m_brain->arbitrate();
			EStatus status = m_brain->Process();

			DrawDebugLine(GetWorld(), startPoint, firstControlPoint, FColor::Green, false, -1.0f, 0, 5.0f);
			DrawDebugLine(GetWorld(), endPoint, secondControlPoint, FColor::Green, false, -1.0f, 0, 5.0f);
			DrawDebugLine(GetWorld(), lastVectorStartPoint, lastVectorEndPoint, FColor::Green, false, -1.0f, 0, 5.0f);

			//DrawDebugBox(GetWorld(), orthogonalVector, FVector(25.0f, 25.0f, 25.0f), FColor::Green);
		}
	}
}

AIrex* AAIIrex::GetNPC() const
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

FVector AAIIrex::getTargetPosition() const
{
	return m_targetPosition;
}

float AAIIrex::getTurnRate() const
{
	return m_turnRate;
}

void AAIIrex::setIsFollowingPath(bool t_bIsFollowingPath)
{
	m_bIsFollowingPath = t_bIsFollowingPath;
}

void AAIIrex::setTargetPosition(FVector t_targetPosition)
{
	m_targetPosition = t_targetPosition;
}

void AAIIrex::setTurnRate(float t_turnRate)
{
	m_turnRate = t_turnRate;
}

void AAIIrex::setMovementBehaviors(float t_deltaTime)
{
	FVector NPCPosition = FVector(m_npcCharacter->GetActorLocation().X,
	                              m_npcCharacter->GetActorLocation().Y,
								  0.0f);

	FVector NPCForwardVector = m_npcCharacter->GetActorForwardVector();
	float NPCMaxWalkSpeed = m_npcCharacter->GetCharacterMovement()->MaxWalkSpeed;

	FRotator nextNPCRotation = UKismetMathLibrary::FindLookAtRotation(NPCPosition, m_targetPosition);
	m_npcCharacter->SetActorRotation(UKismetMathLibrary::RInterpTo(
		m_npcCharacter->GetActorRotation(),
		nextNPCRotation,
		t_deltaTime,
		1.2f
	));

	// We add a translation forward to the NPC
	m_npcCharacter->SetActorLocation(
		m_npcCharacter->GetActorLocation() + 
		(NPCForwardVector * NPCMaxWalkSpeed)
		* t_deltaTime
	);
}
