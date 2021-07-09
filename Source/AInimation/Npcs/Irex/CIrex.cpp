#include "CIrex.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACIrex::ACIrex()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialisation
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(50.0f, 50.0f);

	GetCharacterMovement()->MaxWalkSpeed = 323.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 100.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;

}

// Called when the game starts or when spawned
void ACIrex::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACIrex::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

