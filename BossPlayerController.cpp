// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPlayerController.h"
#include "Blueprint/UserWidget.h"

void ABossPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (BossHUDOverlayAsset)
	{
		BossHUDOverlay = CreateWidget<UUserWidget>(this, BossHUDOverlayAsset);
	}
	BossHUDOverlay->AddToViewport();
	BossHUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WBossEnemyHealthBar)
	{
		BossEnemyHealthBar = CreateWidget<UUserWidget>(this, WBossEnemyHealthBar);
		if (BossEnemyHealthBar)
		{
			BossEnemyHealthBar->AddToViewport();
			BossEnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.f, 0.f);
		BossEnemyHealthBar->SetAlignmentInViewport(Alignment);
	}
}

void ABossPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BossEnemyHealthBar)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(BossEnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 85.f;

		FVector2D SizeInViewport(300.f, 25.f);

		BossEnemyHealthBar->SetPositionInViewport(PositionInViewport);
		BossEnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}
void ABossPlayerController::DisplayEnemyHealthBar()
{
	if (BossEnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		BossEnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}
void ABossPlayerController::RemoveEnemyHealthBar()
{
	if (BossEnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		BossEnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}