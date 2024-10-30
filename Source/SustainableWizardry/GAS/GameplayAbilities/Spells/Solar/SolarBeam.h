// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SustainableWizardry/GAS/GameplayAbilities/DamageAbility/SusWizDamageGameplayAbility.h"
#include "SolarBeam.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API USolarBeam : public USusWizDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UPROPERTY(BlueprintReadWrite)
	int32 NumberOfBeams;

	UFUNCTION(BlueprintCallable)
	void SetNumberOfBeamsToSpawn(int32 Level);

	UFUNCTION(BlueprintCallable)
	int32 GetNumberOfBeamsToSpawn() {return NumberOfBeams; };

	UFUNCTION(BlueprintCallable)
	int32 GetNumberOfBeamsAtLevel(int32 Level);
	
};
