// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/GAS/Data/AttributeInfo.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

	// Get attribute set for getting attribute values
	USusWizAttributeSet* AS = CastChecked<USusWizAttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);

		// Error Log each bind operation
		UE_LOG(LogTemp, Log, TEXT("Binding Callbacks For Tag: %s, Attribute: %s"),
				*Pair.Key.ToString(), *Pair.Value().AttributeName);
	}
	
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// Get attribute set for getting attribute values
	USusWizAttributeSet* AS = CastChecked<USusWizAttributeSet>(AttributeSet);

	check(AttributeInfo)

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	
	BruteForceBroadcast();
	
	
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FSusWizAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BruteForceBroadcast() const
{
	USusWizAttributeSet* AS = CastChecked<USusWizAttributeSet>(AttributeSet);
	
	// TODO: FIX certain attributes from not connecting properly
	// BELOW: Brute force way for accessing each attribute's info through tag that overcomes bug.
	FSusWizAttributeInfo HealthInfo = AttributeInfo->FindAttributeInfoForTag(FSusWizGameplayTags::Get().Attributes_Vital_Health);
	// // Set the information we hold about the attribute to it's value in the attribute set
	HealthInfo.AttributeValue = AS->GetHealth();
	// // Broadcast that info
	AttributeInfoDelegate.Broadcast(HealthInfo);


	FSusWizAttributeInfo EnergyInfo = AttributeInfo->FindAttributeInfoForTag(FSusWizGameplayTags::Get().Attributes_Vital_Energy);
	EnergyInfo.AttributeValue = AS->GetEnergy();
	AttributeInfoDelegate.Broadcast(EnergyInfo);

	FSusWizAttributeInfo MaxHealthInfo = AttributeInfo->FindAttributeInfoForTag(FSusWizGameplayTags::Get().Attributes_Secondary_MaxHealth);
	MaxHealthInfo.AttributeValue = AS->GetMaxHealth();
	AttributeInfoDelegate.Broadcast(MaxHealthInfo);

	FSusWizAttributeInfo MaxEnergyInfo = AttributeInfo->FindAttributeInfoForTag(FSusWizGameplayTags::Get().Attributes_Secondary_MaxEnergy);
	MaxEnergyInfo.AttributeValue = AS->GetMaxEnergy();
	AttributeInfoDelegate.Broadcast(MaxEnergyInfo);

	
}
