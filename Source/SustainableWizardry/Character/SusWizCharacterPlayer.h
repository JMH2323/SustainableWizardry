// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SusWizCharacterBase.h"
#include "SustainableWizardry/Interaction/PlayerInterface.h"

#include "SusWizCharacterPlayer.generated.h"


/**
 * 
 */
class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizCharacterPlayer : public ASusWizCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	ASusWizCharacterPlayer();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	
	/** Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	/** end Player Interface */

	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/* End Combat Interface */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	
	
	

private:
	// 6.0 overrided from character base. Not needed at first
	void InitAbilityActorInfo() override;
	
	

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
