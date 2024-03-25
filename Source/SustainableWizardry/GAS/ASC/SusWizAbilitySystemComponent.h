// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SusWizAbilitySystemComponent.generated.h"

/**
 * 
 */


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/)

UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	// 6.1
	// Now that Ability systems has info, it can bind to delegates and do other nifty stuff.
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	
	
protected:

	// 6.00
	// We want to find the post of when an effect is applied and refer to parent for guidance.
	// But we're going to need initialization, go-to CharacterBase and virtual InitAbility...
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
