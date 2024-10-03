// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "SustainableWizardry/UI/ViewModel/MVVM_LoadScreen.h"
#include "SustainableWizardry/UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();

	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();

	LoadScreenViewModel->LoadData();
	
}
