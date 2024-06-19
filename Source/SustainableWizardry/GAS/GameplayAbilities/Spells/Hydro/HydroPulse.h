// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/DamageAbility/SusWizDamageGameplayAbility.h"
#include "HydroPulse.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API UHydroPulse : public USusWizDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StoreTraceDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerPlayerController();
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector TraceHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> TraceHitActor;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	
};
