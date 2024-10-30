// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizUserWidget.h"

void USusWizUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void USusWizUserWidget::CreatePausedTimer()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("PausedTimerEvent"));

	// Set timer to tick even when the game is paused
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, 
		TimerDelegate, 
		10.0f, 
		false
	);

	// Ensure timer works when paused
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		TimerDelegate,
		10.0f,
		false,
		true // This ensures the timer runs even when paused
	);
}
