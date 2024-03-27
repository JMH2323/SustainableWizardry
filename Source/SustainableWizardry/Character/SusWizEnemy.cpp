// Fill out your copyright notice in the Description page of Project Settings.



#include "SusWizEnemy.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "SustainableWizardry/UI/Widget/UserWidget/SusWizUserWidget.h"
#include "Components/WidgetComponent.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"

ASusWizEnemy::ASusWizEnemy()
{

	AbilitySystemComponent = CreateDefaultSubobject<USusWizAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USusWizAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
}

int32 ASusWizEnemy::GetPlayerLevel()
{
	return Level;
}

void ASusWizEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (USusWizUserWidget* SusWizUserWidget = Cast<USusWizUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		SusWizUserWidget->SetWidgetController(this);
	}

	if (const USusWizAttributeSet* SusWizAS = Cast<USusWizAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SusWizAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SusWizAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(SusWizAS->GetHealth());
		OnMaxHealthChanged.Broadcast(SusWizAS->GetMaxHealth());
	}
	
}

void ASusWizEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	InitializeDefaultAttributes();
}

void ASusWizEnemy::InitializeDefaultAttributes() const
{
	USusWizAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}