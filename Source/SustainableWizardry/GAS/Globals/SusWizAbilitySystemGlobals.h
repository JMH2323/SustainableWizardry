// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "SusWizAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
