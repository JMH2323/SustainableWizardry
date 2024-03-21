// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSusWizAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	// Text for widget user. Sending text to the widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// Dont want to edit it in data. so no edit defaults
	// It's public so we can set the value here in C++
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

UCLASS()
class SUSTAINABLEWIZARDRY_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	// Set from blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSusWizAttributeInfo> AttributeInformation;

	// Function that can get info from the tag
	FSusWizAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
	
	
};
