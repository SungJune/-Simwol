// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "AIController.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossAgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BossAgroSphere"));
	BossAgroSphere->SetupAttachment(GetRootComponent());
	BossAgroSphere->InitSphereRadius(600.f);

	BossCombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BossCombatSphere"));
	BossCombatSphere->SetupAttachment(GetRootComponent());
	BossCombatSphere->InitSphereRadius(100.f);

	BossHealth = 100.f;
	BossMaxHealth = 300.f;

	bOverlappingCombatSphere = false;
	bHasValidTarget = false;

	AttackMinTime = 1.f;
	AttackMaxTime = 7.f;

	
}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	BossAIController = Cast<AAIController>(GetController());//11월 13일

	BossAgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::BossAgroSphereOnOverlapBegin);
	BossAgroSphere->OnComponentEndOverlap.AddDynamic(this, &ABossEnemy::BossAgroSphereOnOverlapEnd);

	BossCombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::BossCombatSphereOnOverlapBegin);
	BossCombatSphere->OnComponentEndOverlap.AddDynamic(this, &ABossEnemy::BossCombatSphereOnOverlapEnd);
	
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossEnemy::BossAttack()
{
	if (!bAttacking)
	{
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && BossCombatMontage)
		{
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(BossCombatMontage, 1.7f);
				AnimInstance->Montage_JumpToSection(FName("Attack"), BossCombatMontage);
				break;

			case 1:
				AnimInstance->Montage_Play(BossCombatMontage, 2.1f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), BossCombatMontage);
				break;

			default:
				;
			}
		}

		if (BossHealth*0.7f)
		{
			//bAttacking = true;
			if (AnimInstance && BossPatonCombatMontage && BossCombatMontage)
			{
				int32 PatonSection = FMath::RandRange(0, 1);
				switch (PatonSection)
				{
				case 0:
					AnimInstance->Montage_Play(BossPatonCombatMontage, 2.0f);
					AnimInstance->Montage_JumpToSection(FName("PatonAttack"), BossPatonCombatMontage);
					break;

				case 1:
					AnimInstance->Montage_Play(BossPatonCombatMontage, 2.2f);
					AnimInstance->Montage_JumpToSection(FName("PatonAttack_1"), BossPatonCombatMontage);
					break;

				case 2:
					AnimInstance->Montage_Play(BossCombatMontage, 1.7f);
					AnimInstance->Montage_JumpToSection(FName("Attack"), BossCombatMontage);
					break;
				case 3:
					AnimInstance->Montage_Play(BossCombatMontage, 2.1f);
					AnimInstance->Montage_JumpToSection(FName("Attack_2"), BossCombatMontage);
					break;

				default:
					;
				}
			}
		}
	}
}

void ABossEnemy::BossAttackEnd()
{
	bAttacking = false;
	if (bOverlappingCombatSphere)
	{
		// 공격속도를 시간차로 조절
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ABossEnemy::BossAttack, AttackTime);
		BossAttack();
	}
}

void ABossEnemy::BossAgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//11월 13일
	if (OtherActor)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		if (Character)
		{
			MoveToTarget(Character);
		}
	}
}

void ABossEnemy::BossAgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//11월 13일
	if (OtherActor)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		if (Character)
		{
			bHasValidTarget = false;
			/*
			if (Character->CombatTarget == this)
			{

			}
			*/
			if (BossAIController)
			{
				BossAIController->StopMovement();
			}
		}
	}
}

void ABossEnemy::BossCombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void ABossEnemy::BossCombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


//11월 13일
void ABossEnemy::MoveToTarget(class AMainCharacter* Target)
{
	if (BossAIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;
		BossAIController->MoveTo(MoveRequest, &NavPath);
	}
}

//11월 13일
void ABossEnemy::Disappear()
{
	Destroy();
}