// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle			UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DeplayName = "Attacking"),
	EMS_Dead			UMETA(DeplayName = "Dead"),

	EMS_MAX				UMETA(DeplayName = "DefaultMAX")
};

UCLASS()
class FIRSTGAMES_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	bool bHasValidTarget;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;
	
	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AI")
	class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AI")
	class USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadwrite,Category = "AI")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	 USoundCue* SwingSound;

	UPROPERTY(VisibleAnywhere,BlueprintReadwrite,Category= "Combat")
	class UBoxComponent* CombatCollision;
	
	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category= "Combat")
	class UAnimMontage* CombatMontage;

	// 적의 공격지연 
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category= "Combat")
	float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	float AttackMaxTime;

	// 몬스터 공격에 플레이어에게 피해를주는 범주 
	UPROPERTY(EditAnywhere, BlueprintReadwrite,Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category = "Combat")
	float DeathDelay;

	

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
	
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainCharacter* Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite,Category ="AI")
	bool bOverlappingCombatSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "AI")
	AMainCharacter* CombatTarget;

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator, AActor * DamageCauser) override;

	void Die(AActor* Causer);

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	bool Allive();

	void Disappear();

};
