// Fill out your copyright notice in the Description page of Project Settings.

// HUD Cpp 파일 

// HUD 작성시 비주얼 스튜디오 프로젝트 이름 밑에있는 Build.cs에서 추가할것 
//PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "UMG"});
//PrivateDependencyModuleNames.AddRange(new string[]{ "Slate", "SlateCore" });

#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//  BP_HUD 에서	오버레이 에셋을 선택했는지 확인하기 위해먼저 실행
	
	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	// 화면에 뷰 를 띄움
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		// View 정렬
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
	
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}
void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		// 몬스터의 위치를 찾아야함 
		// 그러기위해서는 3D 월드의 공간에서 화면으로 변환 하기위해
		//x,y 좌표를 나타내는 2차원 Vector 사용
		FVector2D PositionViewport;

		// 월드위치를 스크린에 투사할수있는기능 
		ProjectWorldLocationToScreen(EnemyLocation, PositionViewport);
		PositionViewport.Y -= 85.f;

		// 투사된 위젯의 크기 설정
		FVector2D SizeViewport(300.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeViewport);
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;

	}
}
void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		GameModeOnly();
		bShowMouseCursor = false;

		bPauseMenuVisible = false;
		//PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}
void AMainPlayerController::GameModeOnly()
{
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}