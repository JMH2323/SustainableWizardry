// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizWidgetController.h"

void USusWizWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void USusWizWidgetController::BroadcastInitialValues()
{
	// Empty for override.
}

void USusWizWidgetController::BindCallbacksToDependencies()
{
	// Empty for override.
}
