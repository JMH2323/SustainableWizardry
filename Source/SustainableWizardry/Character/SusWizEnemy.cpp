// Fill out your copyright notice in the Description page of Project Settings.



#include "SusWizEnemy.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"

ASusWizEnemy::ASusWizEnemy()
{

	AbilitySystemComponent = CreateDefaultSubobject<USusWizAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USusWizAttributeSet>("AttributeSet");

	
}

void ASusWizEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void ASusWizEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
}
