// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"
#include "SusWizGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	mutable FGameplayTag CurrentInputTag;

	UPROPERTY(EditAnywhere, BlueprintType)
	bool isLeftHanded = false;

	UFUNCTION(BlueprintCallable)
	void setLeftHanded() { isLeftHanded = true; }
	
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);


protected:

	float GetEnergyCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
	
};
