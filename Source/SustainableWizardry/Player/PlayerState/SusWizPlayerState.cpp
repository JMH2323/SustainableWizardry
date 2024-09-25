// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizPlayerState.h"

#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "Net/UnrealNetwork.h"

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

void ASusWizPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASusWizPlayerState, Level);
	DOREPLIFETIME(ASusWizPlayerState, XP);
	DOREPLIFETIME(ASusWizPlayerState, AttributePoints);
	DOREPLIFETIME(ASusWizPlayerState, SpellPoints);
}


void ASusWizPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ASusWizPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void ASusWizPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ASusWizPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void ASusWizPlayerState::SetSpellPoints(int32 InPoints)
{
	SpellPoints = InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ASusWizPlayerState::SetLocation(FVector InLocation)
{
	GetPawn()->SetActorLocation(InLocation);
}

void ASusWizPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void ASusWizPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void ASusWizPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ASusWizPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ASusWizPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ASusWizPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
