// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAbilitySystemGlobals.h"

#include "SustainableWizardry/GAS/GameplayAbilities/SusWizAbilityTypes.h"

FGameplayEffectContext* USusWizAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FSusWizGameplayEffectContext();
}
