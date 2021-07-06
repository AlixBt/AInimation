

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CIrex.generated.h"

UCLASS()
class AINIMATION_API ACIrex : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	ACIrex();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
