// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"

void USusWizPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (USusWizAbilitySystemComponent* SusWizASC = Cast<USusWizAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		SusWizASC->DeactivatePassiveAbility.AddUObject(this, &USusWizPassiveAbility::ReceiveDeactivate);
	}
	
}

void USusWizPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
}
