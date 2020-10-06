// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "AIController.h"
#include "Engine/World.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Spawn 의 위치에 등장 위치 
FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector origin = SpawningBox->GetComponentLocation();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(origin, Extent);

	return Point;
}

void ASpawnVolume::SpawnMoveToTarget(class AMainCharacter* Target)
{
	AEnemy* Enemy = Cast<AEnemy>(Target);
	if (Enemy)
	{
		Enemy->SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

		if (Enemy->AIController)
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalActor(Target);
			MoveRequest.SetAcceptanceRadius(10.0f);

			FNavPathSharedPtr NavPath;
			Enemy->AIController->MoveTo(MoveRequest, &NavPath);
		}
		
	}

}

void ASpawnVolume::SpawnOurPawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		UWorld* world = GetWorld();
		FActorSpawnParameters SpawnParams;

		if (world)
		{
			AEnemy* EnemySpawned = world->SpawnActor<AEnemy>(ToSpawn, Location, FRotator(0.f), SpawnParams);
		}
	}
}