// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SusWizCharacterBase.h"
#include "SusWizEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizEnemy : public ASusWizCharacterBase
{
	GENERATED_BODY()

public:
	ASusWizEnemy();

	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */

protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	
};
