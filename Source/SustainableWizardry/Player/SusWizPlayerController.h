// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "SusWizPlayerController.generated.h"


class ASolarBeamDecal;
/**
 * 
 */
class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class USusWizInputConfig;
class USusWizAbilitySystemComponent;
UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASusWizPlayerController();

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bDodgedHit, bool bCrit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(BlueprintCallable)
	FVector GetMagicCircleLocation();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	bool IsInputLeftHanded(FGameplayTag Input);

private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> SusWizContext;


	/** First person camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//UCameraComponent* FirstPersonCameraComponent;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	void Look(const FInputActionValue& Value);
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	void Move(const FInputActionValue& InputActionValue);


	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<USusWizInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<USusWizAbilitySystemComponent> SusWizAbilitySystemComponent;

	USusWizAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASolarBeamDecal> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<ASolarBeamDecal> MagicCircle;

	void UpdateMagicCircleLocation();
	
};
