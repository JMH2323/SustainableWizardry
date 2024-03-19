// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizCharacterPlayer.h"

#include "SustainableWizardry/Player/SusWizPlayerController.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/Character/SusWizCharacterPlayer.h"
#include "SustainableWizardry/UI/HUD/SusWizHUD.h"


ASusWizCharacterPlayer::ASusWizCharacterPlayer()
{
	
}

void ASusWizCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server and HUD
	// If this actor is possessed by a player, we access the player's abilities
	InitAbilityActorInfo();
}

void ASusWizCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init info for the client
	InitAbilityActorInfo();
	
	
}

int32 ASusWizCharacterPlayer::GetPlayerLevel()
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

