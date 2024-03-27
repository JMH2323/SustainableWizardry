// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SuzWizGameModeBase.generated.h"

/**
 * 
 */
class UCharacterClassInfo;
UCLASS()
class SUSTAINABLEWIZARDRY_API ASuzWizGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
};
