// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SusWizGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditAnywhere, BlueprintType)
	bool isLeftHanded = false;

	UFUNCTION(BlueprintCallable)
	void setLeftHanded() { isLeftHanded = true; }
	
};
