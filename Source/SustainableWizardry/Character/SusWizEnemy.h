// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SusWizCharacterBase.h"
#include "SustainableWizardry/UI/Widget/WidgetController/OverlayWidgetController.h"

#include "SusWizEnemy.generated.h"

/**
 * 
 */

class UWidgetComponent;
class UBehaviorTree;
class ASusWizAIController;
UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizEnemy : public ASusWizCharacterBase
{
	GENERATED_BODY()

public:
	ASusWizEnemy();
	virtual void PossessedBy(AController* NewController) override;

	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
	/* End Combat Interface */

	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float Lifespan = 5.f;

protected:

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ASusWizAIController> SusWizAIController;

	
	
};
