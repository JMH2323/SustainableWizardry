// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAbilitySystemComponent.h"

#include "AbilitySystemComponent.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SustainableWizardry/GAS/GameplayAbilities/SusWizGameplayAbility.h"


void USusWizAbilitySystemComponent::AbilityActorInfoSet()
{

	// 6.3 Bind to delegate
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &USusWizAbilitySystemComponent::ClientEffectApplied);

	// 16 testing for getting tags
	const FSusWizGameplayTags& GameplayTags = FSusWizGameplayTags::Get();
	//GameplayTags.Attributes_Secondary_Armor
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
	
	
}

void USusWizAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{

	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const USusWizGameplayAbility* SusWizAbility = Cast<USusWizGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(SusWizAbility->StartupInputTag);
			// Give ability to player
			GiveAbility(AbilitySpec);
		}

		

		
		
		
	}
	
}

void USusWizAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void USusWizAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void USusWizAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                  const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{


	// 6.4 Debug message to check effect applied
	// Can launch in Debug to place Breakpoint and check data
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString("Effect Applied!"));

	// 6.5
	// Want to grab all tags and feed them to our stuff, like the player or, better, the HUD
	// We can't use a TArray for tags, remember we have them already stored in Data. So we use the container
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	// 7
	EffectAssetTags.Broadcast(TagContainer);

	// 6.5 DEBUG TO GET ASSET TAGS THROUGH DEBUG
	// for (const FGameplayTag& Tag : TagContainer)
	// {
	// 	// TODONE: BROADCAST TO WIDGET CONTROLLER
	// 	const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
	// 	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, Msg);
	// }
	
}
