// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizGameplayAbility.h"

FString USusWizGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - "
	"LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum"
	" LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString USusWizGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> "
							 "\n<Default>Causes Upgrade. </>"), Level);
}

FString USusWizGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}