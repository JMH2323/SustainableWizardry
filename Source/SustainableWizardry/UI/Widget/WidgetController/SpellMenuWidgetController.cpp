// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellMenuWidgetController.h"

#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetSusWizPS()->GetSpellPoints());
	
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetSusWizASC()->AbilityStatusChanged.AddLambda([this] (const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if(AbilityInfo)
		{
			FSusWizAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetSusWizPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChanged.Broadcast(SpellPoints);
	});

	
}