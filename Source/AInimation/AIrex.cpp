#include "AIrex.h"

UAIrex::UAIrex()
{
	// Initialisation
	SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);

	m_npcCharacter = nullptr;
}

void UAIrex::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// We catch the owner pawn and cast it to the corresponding class
	APawn* owner = TryGetPawnOwner();

	if (!owner)
	{
		return;
	}

	if (owner->IsA(ACIrex::StaticClass()))
	{
		m_npcCharacter = Cast<ACIrex>(owner);
	}
}

void UAIrex::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UAIrex::NativeUpdateAnimation(float p_deltaSeconds)
{
	Super::NativeUpdateAnimation(p_deltaSeconds);

	// Main update loop
	if (m_npcCharacter)
	{

	}
}
