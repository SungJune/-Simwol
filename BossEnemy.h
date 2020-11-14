// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossEnemy.generated.h"

UCLASS()
class OPENRPG_API ABossEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemy();

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



	bool bHasValidTarget;

	void BossAttack();
	void BossAttackEnd();

	void Disappear();

	FTimerHandle AttackTimer;




};
