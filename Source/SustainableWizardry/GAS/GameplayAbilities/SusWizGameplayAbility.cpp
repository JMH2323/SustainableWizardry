// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizGameplayAbility.h"

#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"


FString USusWizGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Not Included in DEMO "
	, Level);
}

FString USusWizGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Not Included in DEMO "
	, Level);
}

FString USusWizGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

float USusWizGameplayAbility::GetEnergyCost(float InLevel) const
{
	float EnergyCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == USusWizAttributeSet::GetEnergyAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, EnergyCost);
				break;
			}
		}
	}
	return EnergyCost;
}


float USusWizGameplayAbility::GetCooldown(float InLevel) const
{

	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
	
}
