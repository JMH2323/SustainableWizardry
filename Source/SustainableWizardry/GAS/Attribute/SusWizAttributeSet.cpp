// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"

USusWizAttributeSet::USusWizAttributeSet()
{
	
}

void USusWizAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void USusWizAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}






/*
 * Attribute stuff start
 */

void USusWizAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// All Replicated Attributes

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Flare, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Deep, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Seismic, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Swift, COND_None, REPNOTIFY_Always);
	

	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, ArmorPen, COND_None, REPNOTIFY_Always);
	// TODO: JEFF AND ALEX

	
	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USusWizAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
}


/*
 * Primary
 */

void USusWizAttributeSet::OnRep_Flare(const FGameplayAttributeData& OldFlare) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Flare, OldFlare);
}

void USusWizAttributeSet::OnRep_Deep(const FGameplayAttributeData& OldDeep) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Deep, OldDeep);
}

void USusWizAttributeSet::OnRep_Seismic(const FGameplayAttributeData& OldSeismic) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Seismic, OldSeismic);
}

void USusWizAttributeSet::OnRep_Swift(const FGameplayAttributeData& OldSwift) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Swift, OldSwift);
}

/*
 * Secondary
 */

void USusWizAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Armor, OldArmor);
}

void USusWizAttributeSet::OnRep_ArmorPen(const FGameplayAttributeData& OldArmorPen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, ArmorPen, OldArmorPen);
}

//TODO: JEFF ALEX

/*
 * Vital
 */

void USusWizAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, Health, OldHealth);
}

void USusWizAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, MaxHealth, OldMaxHealth);
}

void USusWizAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, MaxHealth, OldEnergy);
}

void USusWizAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USusWizAttributeSet, MaxHealth, OldMaxEnergy);
}

