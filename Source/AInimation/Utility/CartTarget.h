#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CartTarget.generated.h"

UCLASS()
class AINIMATION_API ACartTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACartTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
