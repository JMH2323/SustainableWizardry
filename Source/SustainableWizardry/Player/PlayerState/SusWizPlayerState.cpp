// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizPlayerState.h"

#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"

ASusWizPlayerState::ASusWizPlayerState()
{

	// The rate per second at which values are verified.
	NetUpdateFrequency = 100.f;


	AbilitySystemComponent = CreateDefaultSubobject<USusWizAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USusWizAttributeSet>("AttributeSet");


	
	
}




UAbilitySystemComponent* ASusWizPlayerState::GetAbilitySystemComponent() const
{

	return AbilitySystemComponent;
	
}
