// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossEnemy.generated.h"


UENUM(BlueprintType)
enum class EBossEnemyMovementStatus : uint8
{
	EMS_Idle				UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget		UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking			UMETA(DeplayName = "Attacking"),
	EMS_Dead				UMETA(DeplayName = "Dead"),

	EMS_MAX					UMETA(DeplayNmae = "DefaultMAX")
};


UCLASS()
class OPENRPG_API ABossEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EBossEnemyMovementStatus BossEnemyMovementStatus;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* BossAgroSphere;

	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* BossCombatSphere;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	class UAnimMontage* BossCombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	class UAnimMontage* BossPatonCombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float BossHealth;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float BossMaxHealth;

	
	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* BossAIController;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float BossDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float BossExp;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "AI")
	AMainCharacter* BossCombatTarget;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainCharacter* Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "AI")
	bool bOverlappingCombatSphere;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	float AttackMaxTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Combat")
	class UBoxComponent* RightCombatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Combat")
	class UBoxComponent* LeftCombatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Combat")
	class UBoxComponent* MouseCombatCollision;


	FORCEINLINE void SetBossEnemyMovementStatus(EBossEnemyMovementStatus Status) { BossEnemyMovementStatus = Status; }
	FORCEINLINE EBossEnemyMovementStatus GetBossEnemyMovementStatus() { return BossEnemyMovementStatus; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
		virtual void BossAgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void BossAgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void BossCombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void BossCombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void BossCombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void BossCombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable)
	void ActivateCollision_a();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision_a();

	UFUNCTION(BlueprintCallable)
	void ActivateCollision_b();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision_b();

	UFUNCTION(BlueprintCallable)
	void ActivateCollision_c();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision_c();

	UFUNCTION(BlueprintCallable)
	void ActivateCollision_m();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision_m();

	UFUNCTION(BlueprintCallable)
	void ActivateCollision_Paton();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision_Paton();

	UFUNCTION(BlueprintCallable)
	void ActivateCollision_Paton_a();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision_Paton_a();

	bool bHasValidTarget;

	void BossAttack();

	UFUNCTION(BlueprintCallable)
	void BossAttackEnd();

	void Disappear();

	bool Alive();

	FTimerHandle AttackTimer;

	FTimerHandle DeathTimer;

	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	float DeathDlay;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeclass;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;



};
