// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTGAMES_API AExplosive : public AItem
{
	GENERATED_BODY()

public:
		AExplosive();

		UPROPERTY(EditAnywhere,BlueprintReadwrite,Category ="Damage")
		float Damage;

		UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)override;

		UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	
		UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;
	
};
