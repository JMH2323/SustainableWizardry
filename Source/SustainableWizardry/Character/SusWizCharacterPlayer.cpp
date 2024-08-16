// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizCharacterPlayer.h"

#include "SustainableWizardry/Player/SusWizPlayerController.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/Character/SusWizCharacterPlayer.h"
#include "SustainableWizardry/GAS/Data/LevelUpInfo.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SustainableWizardry/GAS/Data/CharacterClassInfo.h"
#include "SustainableWizardry/UI/HUD/SusWizHUD.h"


ASusWizCharacterPlayer::ASusWizCharacterPlayer()
{
	CharacterClass = ECharacterClass::Wizard;
	

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void ASusWizCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server and HUD
	// If this actor is possessed by a player, we access the player's abilities
	InitAbilityActorInfo();

	AddCharacterAbilities();
	
}

void ASusWizCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init info for the client
	InitAbilityActorInfo();
	
	
}

int32 ASusWizCharacterPlayer::GetAttributePoints_Implementation() const
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->GetAttributePoints();
}

int32 ASusWizCharacterPlayer::GetSpellPoints_Implementation() const
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->GetSpellPoints();
}

void ASusWizCharacterPlayer::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		SusWizPlayerController->ShowMagicCircle(DecalMaterial);
	}
	
}

void ASusWizCharacterPlayer::HideMagicCircle_Implementation()
{
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		SusWizPlayerController->HideMagicCircle();
	}
}

FVector ASusWizCharacterPlayer::GetMagicCircleLocation_Implementation()
{
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		return SusWizPlayerController->GetMagicCircleLocation();
	}
	else
	{
		FVector Failcase;
		return Failcase;
	}
}

void ASusWizCharacterPlayer::AddToXP_Implementation(int32 InXP)
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	SusWizPlayerState->AddToXP(InXP);
}

void ASusWizCharacterPlayer::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void ASusWizCharacterPlayer::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 ASusWizCharacterPlayer::GetXP_Implementation() const
{
	const ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->GetXP();
}

int32 ASusWizCharacterPlayer::FindLevelForXP_Implementation(int32 InXP) const
{
	const ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ASusWizCharacterPlayer::GetAttributePointsReward_Implementation(int32 Level) const
{
	const ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 ASusWizCharacterPlayer::GetSpellPointsReward_Implementation(int32 Level) const
{
	const ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void ASusWizCharacterPlayer::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	SusWizPlayerState->AddToLevel(InPlayerLevel);
	if (USusWizAbilitySystemComponent* SusWizASC = Cast<USusWizAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		SusWizASC->UpdateAbilityStatuses(SusWizPlayerState->GetPlayerLevel());
	}
	
}

void ASusWizCharacterPlayer::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	SusWizPlayerState->AddToAttributePoints(InAttributePoints);
}

void ASusWizCharacterPlayer::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	SusWizPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 ASusWizCharacterPlayer::GetPlayerLevel_Implementation()
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState)
	return SusWizPlayerState->GetPlayerLevel();
}

void ASusWizCharacterPlayer::InitAbilityActorInfo()
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	

	if(SusWizPlayerState && SusWizPlayerState->GetAbilitySystemComponent())
	{
		//UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is valid"));

		SusWizPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(SusWizPlayerState, this);
		// 6.1
		Cast<USusWizAbilitySystemComponent>(SusWizPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
		
		AbilitySystemComponent = SusWizPlayerState->GetAbilitySystemComponent();
		AttributeSet = SusWizPlayerState->GetAttributeSet();

		// We can apply in c++ but I choose to apply with gameplay effects.
		//AddCharacterAbilities(); // Adding abilities after successful initialization

		//UE_LOG(LogTemp, Warning, TEXT("%s: InitAbilityActorInfo called. AttributeSet: %s"), *GetName(), AttributeSet ? *AttributeSet->GetName() : TEXT("null"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is NOT valid"));
		// This can happen.
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASusWizCharacterPlayer::InitAbilityActorInfo);
		
	}

	// HUD Stuff
	// Tying in the player into the widgets
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		if (ASusWizHUD* SusWizHUD = Cast<ASusWizHUD>(SusWizPlayerController->GetHUD()))
		{
			SusWizHUD->InitOverlay(SusWizPlayerController, SusWizPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}


	// 11.2 initializing default primary attributes
	// InitializePrimaryAttributes();
	// 12.4 Initialize all attributes
	InitializeDefaultAttributes();

	
};

