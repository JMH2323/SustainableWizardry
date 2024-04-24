// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SusWizCharacterBase.h"
#include "SustainableWizardry/Interaction/PlayerInterface.h"

#include "SusWizCharacterPlayer.generated.h"


/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizCharacterPlayer : public ASusWizCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	ASusWizCharacterPlayer();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Players Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	/** end Player Interface */

	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/* End Combat Interface */
	
	

private:
	// 6.0 overrided from character base. Not needed at first
	void InitAbilityActorInfo() override;
};
