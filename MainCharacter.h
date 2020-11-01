// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName ="Dead")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering	UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_MAX	UMETA(DisplayName = "DefaultMax"),
};

UCLASS()
class OPENRPG_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Combat")
	class UBoxComponent* LeftHandCombatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Combat")
	class UBoxComponent* RightHandCombatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Combat")
	class UBoxComponent* LeftTobaseCombatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Combat")
	class UBoxComponent* RightTobaseCombatCollision;

	// 플레이어가 피격을 당했을때 나온는 이펙트 
	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category = "Combat")
	class UParticleSystem* PlayerHitParticles;

	// 몬스터가 플레이어 떄릴때 사운드 
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
	class USoundCue* EnemyHitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category ="Combat")
	class AEnemy* CombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookupRate;

	// PlayerStat
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Player Stat")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Player Stat")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stat")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Player Stat")
	float Stamina;

	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category = "Player Stat")
	float PlayerExp;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Player Stat")
	float PlayerMaxExp;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "PlayerStat")
	int32 PlayerLevel;

	int ComboAttack_num;

	UPROPERTY(EditDefaultsOnly,Category = Attack)
	UAnimMontage* AttackMeleeAnim;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UAnimMontage* AttackMeleeAnimCombo;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UAnimMontage* AttackMeleeAnimCombo2;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UAnimMontage* AttackCombo;
	
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UAnimMontage* LastAttackCombo;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "AI")
	float Damage;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	
	void LookUpAtRate(float Rate);

	void AttackMelee();

	void DecrementHealth(float Amount);

	void Die();

	void PlayerExpBar();

	void PlayerLevelText(int32 Level);

	FORCEINLINE void SetMovementStatus(EMovementStatus Status) { MovementStatus = Status; }

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator,AActor * DamageCauser) override;

	//virtual void Jump() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	UPROPERTY(VisibleAnywhere,BlueprintReadwrite,Category = "Combat")
	FVector CombatTargetLocation;

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable)
	void ActivateCollisiontwo();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollisiontwo();

	UFUNCTION(BlueprintCallable)
	void ActivateCollisionthree();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollisionthree();

	UFUNCTION(BlueprintCallable)
	void LastActivateCollision();

	UFUNCTION(BlueprintCallable)
	void LastDeactivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UFUNCTION(BlueprintCallable)
	void AttackMeleeEnd();

	void LMBDown();
	void LMBup() { bLMBDown = false; }
	bool bLMBDown;

	// 공격 멈춤 스위치 
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Anims")
	bool isDuringAttack;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Combat")
	bool bHasCombatTarget;

	// 무기 생성 true/false
	bool CanSetWeapon();

	// 적에게 보간 (공격할때 적의 방향을 봄) 
	float InterpSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category = "movement")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "movement")
	float MinSprintStamina;

	bool bInterpToEnemy;

	bool bShiftKeyDown;

	void SetInterpToEnemy(bool Interp);

	FRotator GetLookAtRotaionYaw(FVector Target);

	//void SetWeapon(class AWeaponKatana* NewWeapon);

	//무기 생성(?)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = items)
	class AWeaponKatana* Equippedweapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = items)
	class Aitem* ActiveOverlappingItem;

	//장착할 무기 설정
	void SetEquippedWeapon(AWeaponKatana* WeaponToSet);
	FORCEINLINE AWeaponKatana* GetEquippedWeapon() { return Equippedweapon; }

	//무기 교체 설정
	FORCEINLINE void SetActiveOverlapping(Aitem* item) { ActiveOverlappingItem = item; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera ; }

	// 적 에게 보간 
	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

	UFUNCTION()
	virtual void PlayerCombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void PlayerCombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere,BlueprintReadwrite,Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Combat")
	AController* PauchInstigator;

	FORCEINLINE void SetInstigator(AController* Inst) { PauchInstigator = Inst; }

	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }
	
	


};
