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

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 3;
	
};
