// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SusWizUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API USusWizUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UFUNCTION(BlueprintCallable)
	void CreatePausedTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void PausedTimerEvent();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerHandle;

	
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
