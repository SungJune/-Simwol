// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle				UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget		UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking			UMETA(DeplayName = "Attacking"),
	EMS_Dead				UMETA(DeplayName = "Dead"),

	EMS_MAX					UMETA(DeplayNmae = "DefaultMAX")
};

UCLASS()
class OPENRPG_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category ="AI")
	class USphereComponent* AgroSphere;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* CombatSphere;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere,BlueprintReadwrite,Category = "Combat")
	class UBoxComponent* CombatCollision;

	// 몬스터가 피격을 당했을때 나온는 이펙트 
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	class UParticleSystem* EnemyHitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	class UParticleSystem* EnemyLastHitParticle;

	// 플레이어가 몬스터 공격할때 사운드 
	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category = "Combat")
	class USoundCue* PlayerHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category ="Combat")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditAnywhere,BlueprintReadwrite, Category = "AI")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float Exp;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "PlayerStat")
	int32 PlayerLevel;



	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

	UFUNCTION()
	virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainCharacter* Target);

	UPROPERTY(VisibleAnywhere,BlueprintReadwrite,Category = "AI")
	bool bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Combat")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "AI")
	AMainCharacter* CombatTarget;

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	void Attack();

	void Die();

	void Disappar();

	bool Alive();

	bool bHasValidTarget;

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	FTimerHandle AttackTimer;

	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category ="Combat")
	float DeathDlay;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category ="Combat")
	float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	float AttackMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category ="Combat")
	TSubclassOf<UDamageType> DamageTypeclass;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
};

