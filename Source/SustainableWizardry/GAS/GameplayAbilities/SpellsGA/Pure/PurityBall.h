// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SusWizProjectileSpell.h"
#include "PurityBall.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API UPurityBall : public USusWizProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
