#include "AIIrex.h"
#include "../../IrexGlobalState.h"
#include "../../WanderAndLookForPreyState.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Navigation/PathFollowingComponent.h"
#include "../../Goap/GoalThink.h"

AAIIrex::AAIIrex(FObjectInitializer const& p_objectInitializer) :
	Super(p_objectInitializer),
	m_brain(new GoalThink(this)),
	m_bPreyIsFound(false),
	m_npcCharacter(nullptr),
	m_npcAnimInstance(nullptr),
	m_pNavigationSystem(nullptr),
	m_path(nullptr)
{
	// Initialization
	m_pStateMachine = new StateMachine<AAIIrex>(this);

	m_pStateMachine->SetCurrentState(WanderAndLookForPreyState::Instance());
	m_pStateMachine->SetGlobalState(IrexGlobalState::Instance());

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

	if (m_brain != nullptr)
	{
		m_brain->arbitrate();
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
			m_brain->arbitrate();
			EStatus status = m_brain->Process();
			GEngine->AddOnScreenDebugMessage(0, -1.0f, FColor::Red, FString::Printf(TEXT("Index: %i"), m_path->GetPathIndex()));
			GEngine->AddOnScreenDebugMessage(0, -1.0f, FColor::Yellow, FString::Printf(TEXT("Is following a path: %s"), m_bIsFollowingPath ? TEXT("true") : TEXT("false")));
		}
	}
}

bool AAIIrex::GetPreyIsFound() const
{
	return m_bPreyIsFound;
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

void AAIIrex::FollowPath()
{
	int pathIndex = m_path->GetPathIndex();

	if (pathIndex == m_path->GetPath().Num())
	{
		m_path->SetPathIndex(0);
	}

	EPathFollowingRequestResult::Type pathResult;
	pathResult = MoveToLocation(m_path->GetPathPoint(), 50.0f);

	if (pathResult == 1)
	{
		m_path->IncrementPathIndex();
	}
}

AACPath* AAIIrex::getPath() const
{
	return m_path;
}

bool AAIIrex::getIsFollowingPath() const
{
	return m_bIsFollowingPath;
}

void AAIIrex::setIsFollowingPath(bool t_bIsFollowingPath)
{
	m_bIsFollowingPath = t_bIsFollowingPath;
}
