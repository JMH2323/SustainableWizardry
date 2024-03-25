// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"

// Sets default values
ASusWizCharacterBase::ASusWizCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* ASusWizCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;	
}

// Called when the game starts or when spawned
void ASusWizCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASusWizCharacterBase::InitAbilityActorInfo()
{
	// override in characterplayer
}

void ASusWizCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	// Ran into a Null pointer error, implemented source object during effect application
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
	
}

void ASusWizCharacterBase::InitializeDefaultAttributes() const
{
	// PAY ATTENTION to the order you apply.
	// These values are dependent on each other!
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1);
	ApplyEffectToSelf(DefaultVitalAttributes, 1);
}

void ASusWizCharacterBase::AddCharacterAbilities()
{

	USusWizAbilitySystemComponent* SusWizASC = CastChecked<USusWizAbilitySystemComponent>(AbilitySystemComponent);
		if(!HasAuthority()) return;
	
	SusWizASC->AddCharacterAbilities(StartupAbilities);
	
}


