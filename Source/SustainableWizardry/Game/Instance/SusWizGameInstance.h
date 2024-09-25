// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SusWizGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	FName PlayerStartTag = FName();
	UPROPERTY()
	FString LoadSlotName = FString();
	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
