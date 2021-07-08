

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACPath.generated.h"

UCLASS()
class AINIMATION_API AACPath : public AActor
{
	GENERATED_BODY()

	int m_pathIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (AllowPrivateAccess = "true", MakeEditwidget = "true"))
	TArray<FVector> m_path;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AACPath();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getters
	TArray<FVector> GetPath() const;
	FVector GetPathPoint() const;
	int GetPathIndex() const;

	// Setters
	void SetPathIndex(int p_pathIndex);
	void IncrementPathIndex();
};
