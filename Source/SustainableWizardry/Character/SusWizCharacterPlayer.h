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
	void LoadProgress();
	virtual void OnRep_PlayerState() override;

	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;

	
	
	/** Player Interface */
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual FVector GetMagicCircleLocation_Implementation() override;
	
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	void ResetPlayerProgress();
	/** end Player Interface */

	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	/* End Combat Interface */

	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;
	
	FTimerHandle DeathTimer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	FVector StartingLocation = FVector::ZeroVector;


private:
	// 6.0 overriden from character base. Not needed at first
	void InitAbilityActorInfo() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

protected:
	virtual void BeginPlay() override;
};

inline void ASusWizCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	StartingLocation = GetActorLocation();
}
