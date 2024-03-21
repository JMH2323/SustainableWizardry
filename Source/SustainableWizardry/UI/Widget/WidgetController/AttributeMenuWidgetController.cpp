// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "SustainableWizardry/GAS/Data/AttributeInfo.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	//
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// Get attribute set for getting attribute values
	USusWizAttributeSet* AS = CastChecked<USusWizAttributeSet>(AttributeSet);

	check(AttributeInfo)

	FSusWizAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FSusWizGameplayTags::Get().Attributes_Primary_Flare);

	// Set the information we hold about the attribute to it's value in the attribute set
	Info.AttributeValue = AS->GetFlare();

	// Broadcast that info
	AttributeInfoDelegate.Broadcast(Info);
	
}
