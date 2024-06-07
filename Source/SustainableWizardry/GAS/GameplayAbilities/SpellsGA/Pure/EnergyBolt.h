// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SpellsGA/SusWizProjectileSpell.h"
#include "EnergyBolt.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API UEnergyBolt : public USusWizProjectileSpell
{
	GENERATED_BODY()


public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "EnergyBolt")
	float ProjectileSpread = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "EnergyBolt")
	int32 MaxNumProjectiles = 4;

	UPROPERTY(EditDefaultsOnly, Category = "EnergyBolt")
	int32 NumProjectiles = 0;

	UPROPERTY(EditDefaultsOnly, Category = "EnergyBolt")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "EnergyBolt")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "EnergyBolt")
	bool bLaunchHomingProjectiles = true;

	AActor* HomingTarget = nullptr;
	
};
