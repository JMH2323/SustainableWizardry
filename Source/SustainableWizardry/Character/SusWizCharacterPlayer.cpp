// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizCharacterPlayer.h"

#include "SustainableWizardry/Player/SusWizPlayerController.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
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

void ASusWizCharacterPlayer::InitAbilityActorInfo()
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();


	
	// Tying in the player into the widgets
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		if (ASusWizHUD* SusWizHUD = Cast<ASusWizHUD>(SusWizPlayerController->GetHUD()))
		{
			SusWizHUD->InitOverlay(SusWizPlayerController, SusWizPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
};

