// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"


// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StrartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	bInterping = false;

	InterpSpeed = 4.0f;
	InterpTime = 1.f;

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StrartPoint = GetActorLocation();
	EndPoint += StrartPoint;
	bInterping = false;
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

	Distance = (EndPoint - StrartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		// 보간 함수 (두점의 거리이동을 부드럽게 해주는 수학함수 

		FVector CurrentLocation = GetActorLocation();

		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);

		SetActorLocation(Interp);

		float DistanceTraveled = (GetActorLocation() - StrartPoint).Size();
		if (Distance - DistanceTraveled <=1.f)
		{
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVectors(StrartPoint, EndPoint);
		}
	}
}
void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}

