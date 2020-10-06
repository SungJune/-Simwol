// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));

	bOverlappingCombatSphere = false;

	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 10.f;

	AttackMinTime = 0.5f;
	AttackMaxTime = 5.5f;

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	DeathDlay = 3.f;

	bHasValidTarget = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Alive())
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		if (Character)
		{
			MoveToTarget(Character);
		}
	}
}
void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		{
			if (Character)
			{
				bHasValidTarget = false;
				if (Character->CombatTarget == this)
				{
					//Character->SetHasCombatTarget(false);
					Character->SetCombatTarget(nullptr);
				}
				Character->SetHasCombatTarget(false);
				if (Character->MainPlayerController)
				{
					Character->MainPlayerController->RemoveEnemyHealthBar();
				}
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
				if (AIController)
				{
					AIController->StopMovement();
				}
				
			}
		}
	}
}
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && Alive())
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		{
			if (Character)
			{
				bHasValidTarget = true;
				Character->SetCombatTarget(this);
				Character->SetHasCombatTarget(true);
				if (Character->MainPlayerController)
				{
					Character->MainPlayerController->DisplayEnemyHealthBar();
				}
				CombatTarget = Character;
				bOverlappingCombatSphere = true;
				Attack();
			}
		}
	}
}
void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		{
			if (Character)
			{
				/*
				if (Character->CombatTarget==this)
				{
					//Character->SetHasCombatTarget(false);
					Character->SetCombatTarget(nullptr);
				}
				*/
				bOverlappingCombatSphere = false;
				if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
				{
					MoveToTarget(Character);
					CombatTarget = nullptr;
				}
				GetWorldTimerManager().ClearTimer(AttackTimer);
			}
		}
	}
}

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
		if (Character)
		{
			if (Character->PlayerHitParticles)
			{
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
				if (TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Character->PlayerHitParticles, SocketLocation, FRotator(0.f), false);
				}
			}
			if (Character->EnemyHitSound)
			{
				UGameplayStatics::PlaySound2D(this, Character->EnemyHitSound);
			}
			if (DamageTypeclass)
			{
				UGameplayStatics::ApplyDamage(Character, Damage, AIController, this, DamageTypeclass);
			}
			
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void AEnemy::MoveToTarget(AMainCharacter* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);



		/*
		// 몬스터의 움직임 경로를 보여줌
		auto PathPoints = NavPath->GetPathPoints();

		for (auto Points : PathPoints)
		{
			FVector Location = Points.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
		}
		*/
	}
}

void AEnemy::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{

	if (Alive() && bHasValidTarget)
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
			}
		}
	}
	
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	if (bOverlappingCombatSphere)
	{
		// 공격속도를 시간차로 조절
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		Attack();
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}
	
	return DamageAmount;
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappar, DeathDlay);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappar()
{
	Destroy();
}