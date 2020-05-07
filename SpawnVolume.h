// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class FIRSTGAMES_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Spawning")
	class UBoxComponent* SpawningBox;

	// 블루프린트 이용 
	// 선택된 동물 종류 등을 나타냄( 몬스터 등 )
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category ="Spawning")
	TSubclassOf<AActor> Actor_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> Actor_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> Actor_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> Actor_4;

	TArray<TSubclassOf<AActor>> SpawnArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure,Category="Spawning")
	FVector GetSpawningPoint();

	UFUNCTION(BlueprintPure, Category = "Spawning")
	TSubclassOf<AActor> GetSpawnActor();

	// ----- 하드 코딩 Spawn-----

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "Spawning")
	void SpawnOurActor(UClass* ToSpawn, const FVector& Location);
	
};
