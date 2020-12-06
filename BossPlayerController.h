// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BossPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OPENRPG_API ABossPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "widgets")
	TSubclassOf<class UUserWidget> BossHUDOverlayAsset;

	// Create HUD Widgets
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "widgets")
	UUserWidget* BossHUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "widgets")
	TSubclassOf<UUserWidget> WBossEnemyHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "widgets")
	UUserWidget* BossEnemyHealthBar;

	bool bEnemyHealthBarVisible;

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector BossEnemyLocation;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	
};
