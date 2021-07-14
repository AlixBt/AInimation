#include "CartTarget.h"

// Sets default values
ACartTarget::ACartTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACartTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACartTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

