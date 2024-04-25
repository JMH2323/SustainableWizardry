// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SustainableWizardry/UI/Widget/UserWidget/SusWizUserWidget.h"
#include "SustainableWizardry/UI/Widget/WidgetController/SusWizWidgetController.h"
#include "SustainableWizardry/UI/Widget/WidgetController/AttributeMenuWidgetController.h"
#include "SusWizHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UOverlayWidgetController;
class USpellMenuWidgetController;
/**
 * 
 */

UCLASS()
class SUSTAINABLEWIZARDRY_API ASusWizHUD : public AHUD
{
	GENERATED_BODY()

public:

	

	// Get widget controller
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	// Get Attribute menu widget controller
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
	
	// Get key variables to initialize
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:


	UPROPERTY()
	TObjectPtr<USusWizUserWidget>  OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USusWizUserWidget> OverlayWidgetClass;

	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	
};
