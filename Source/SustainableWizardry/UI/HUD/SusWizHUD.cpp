// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizHUD.h"

#include "Blueprint/UserWidget.h"
#include "SustainableWizardry/UI/Widget/WidgetController/AttributeMenuWidgetController.h"
#include "SustainableWizardry/UI/Widget/WidgetController/OverlayWidgetController.h"
#include "SustainableWizardry/UI/Widget/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* ASusWizHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{

	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		//Bind callbacks
		OverlayWidgetController->BindCallbacksToDependencies();

		
	}
	return OverlayWidgetController;
	
}

UAttributeMenuWidgetController* ASusWizHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);

		//Bind callbacks
		AttributeMenuWidgetController->BindCallbacksToDependencies();

		
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* ASusWizHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}
	return SpellMenuWidgetController;
}

void ASusWizHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{

	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_SusWizHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_SusWizHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<USusWizUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
	
}
