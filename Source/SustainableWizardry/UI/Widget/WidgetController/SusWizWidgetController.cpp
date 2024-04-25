// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizWidgetController.h"

#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/Player/SusWizPlayerController.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"



void USusWizWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void USusWizWidgetController::BroadcastInitialValues()
{
	// Empty for override.
}

void USusWizWidgetController::BindCallbacksToDependencies()
{
	// Empty for override.
}

void USusWizWidgetController::BroadcastAbilityInfo()
{
	if (!GetSusWizASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FSusWizAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(SusWizAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = SusWizAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetSusWizASC()->ForEachAbility(BroadcastDelegate);
}

ASusWizPlayerController* USusWizWidgetController::GetSusWizPC()
{
	if (SusWizPlayerController == nullptr)
	{
		SusWizPlayerController = Cast<ASusWizPlayerController>(PlayerController);
	}
	return SusWizPlayerController;
}

ASusWizPlayerState* USusWizWidgetController::GetSusWizPS()
{
	if (SusWizPlayerState == nullptr)
	{
		SusWizPlayerState = Cast<ASusWizPlayerState>(PlayerState);
	}
	return SusWizPlayerState;
}

USusWizAbilitySystemComponent* USusWizWidgetController::GetSusWizASC()
{
	if (SusWizAbilitySystemComponent == nullptr)
	{
		SusWizAbilitySystemComponent = Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent);
	}
	return SusWizAbilitySystemComponent;
}

USusWizAttributeSet* USusWizWidgetController::GetSusWizAS()
{
	if (SusWizAttributeSet == nullptr)
	{
		SusWizAttributeSet = Cast<USusWizAttributeSet>(AttributeSet);
	}
	return SusWizAttributeSet;
}
