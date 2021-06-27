


#include "ACPath.h"

// Sets default values
AACPath::AACPath() :
	m_pathIndex(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AACPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FVector> AACPath::GetPath() const
{
	return m_path;
}

FVector AACPath::GetPathPoint() const
{
	return m_path[m_pathIndex];
}

int AACPath::GetPathIndex() const
{
	return m_pathIndex;
}

void AACPath::SetPathIndex(int p_pathIndex)
{
	m_pathIndex = p_pathIndex;
}

void AACPath::IncrementPathIndex()
{
	m_pathIndex++;
}

