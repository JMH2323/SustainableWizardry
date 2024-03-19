// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */

// TODO: Alex - MaxEnergy = baseValue + Solar + PlayerLevel ?AND MORE?
// TODO: Jeff - MvmntSpeed = 100 + Swift + PlayerLevel(small) ?AND MORE?


UCLASS()
class SUSTAINABLEWIZARDRY_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	// 14 make constructor for calc
public:
	UMMC_MaxHealth();

	// 14.1 Start this to calculate value
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	// variable for what we want to capture.
	FGameplayEffectAttributeCaptureDefinition DeepDef;

};
