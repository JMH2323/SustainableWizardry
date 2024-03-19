// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizUserWidget.h"

void USusWizUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}