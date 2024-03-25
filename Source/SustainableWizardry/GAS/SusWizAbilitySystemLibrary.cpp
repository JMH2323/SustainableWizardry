// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
#include "SustainableWizardry/UI/HUD/SusWizHUD.h"


// Function for getting the overlay widget controller and all it's relevant GAS data that we might want to display it 
UOverlayWidgetController* USusWizAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{

	// Get and check the player controller by referring to the world context
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		{
		// Get and check the HUD of the player controller
		if(ASusWizHUD* SusWizHUD = Cast<ASusWizHUD>(PC->GetHUD()))
			{
				// Get the player state from the player controller 
				ASusWizPlayerState* PS = PC->GetPlayerState<ASusWizPlayerState>();
			
				// Get the ASC and Attributes from the player state
				UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
				UAttributeSet* AS = PS->GetAttributeSet();

				// Pass the above data and construct the widget controller overlay with this information so we
				// can refer to it and display it to the player.
				const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
				return SusWizHUD->GetOverlayWidgetController(WidgetControllerParams);
			}
		}

	// if none of the above, return null
	return nullptr;
	
}

UAttributeMenuWidgetController* USusWizAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{

	// Get and check the player controller by referring to the world context
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		// Get and check the HUD of the player controller
		if(ASusWizHUD* SusWizHUD = Cast<ASusWizHUD>(PC->GetHUD()))
		{
			// Get the player state from the player controller 
			ASusWizPlayerState* PS = PC->GetPlayerState<ASusWizPlayerState>();
			
			// Get the ASC and Attributes from the player state
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			// Pass the above data and construct the attribute controller overlay with this information so we
			// can refer to it and display it to the player.
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return SusWizHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	// if none of the above, return null
	return nullptr;
	
}
