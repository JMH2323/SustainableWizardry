// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "SusWizPlayerController.generated.h"


/**
 * 
 */
class UInputMappingContext;
UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASusWizPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

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
	
};
