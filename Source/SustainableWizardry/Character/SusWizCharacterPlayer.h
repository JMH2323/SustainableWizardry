// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SusWizCharacterBase.h"
#include "SusWizCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizCharacterPlayer : public ASusWizCharacterBase
{
	GENERATED_BODY()

public:
	ASusWizCharacterPlayer();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;


	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */
	
	

private:
	// 6.0 overrided from character base. Not needed at first
	void InitAbilityActorInfo() override;
};
