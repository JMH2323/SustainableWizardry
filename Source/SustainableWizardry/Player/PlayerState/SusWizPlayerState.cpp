// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizPlayerState.h"

#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"

ASusWizPlayerState::ASusWizPlayerState()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Player state is successfully created."));
	AbilitySystemComponent = CreateDefaultSubobject<USusWizAbilitySystemComponent>("AbilitySystemComponent");
	
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USusWizAttributeSet>("AttributeSet");
	


	// The rate per second at which values are verified.
	NetUpdateFrequency = 100.f;
	
}




UAbilitySystemComponent* ASusWizPlayerState::GetAbilitySystemComponent() const
{

	UE_LOG(LogTemp, Warning, TEXT("Player state is getting ASC."));
	return AbilitySystemComponent;
	
}
