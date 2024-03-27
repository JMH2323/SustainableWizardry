// Copyright Epic Games, Inc. All Rights Reserved.

#include "SustainableWizardryGameMode.h"
#include "SustainableWizardryCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASustainableWizardryGameMode::ASustainableWizardryGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

}
