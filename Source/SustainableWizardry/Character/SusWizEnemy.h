// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SusWizCharacterBase.h"
#include "SustainableWizardry/UI/Widget/WidgetController/OverlayWidgetController.h"
#include "SustainableWizardry/GAS/Data/CharacterClassInfo.h"
#include "SusWizEnemy.generated.h"

/**
 * 
 */

class UWidgetComponent;
UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizEnemy : public ASusWizCharacterBase
{
	GENERATED_BODY()

public:
	ASusWizEnemy();

	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	virtual void Die() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float Lifespan = 5.f;

protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Ranger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	
	
};
