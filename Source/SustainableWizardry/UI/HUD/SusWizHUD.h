// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SustainableWizardry/UI/Widget/UserWidget/SusWizUserWidget.h"
#include "SustainableWizardry/UI/Widget/WidgetController/SusWizWidgetController.h"
#include "SusWizHUD.generated.h"

/**
 * 
 */
class UOverlayWidgetController;
class UAuraUserWidget;
UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<USusWizUserWidget>  OverlayWidget;

	// Get widget controller
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	// Get key variables to initialize
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<USusWizUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	
};
