// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

/*
 *	Saved Actors
 */
USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()
	UPROPERTY()
	FName ActorName = FName();
	UPROPERTY()
	FTransform Transform = FTransform();
	
	// Serialized variables from the Actors (only those marked with SaveGame specifier)
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}


/*
 *	Saved Map
 */
USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()
	UPROPERTY()
	FString MapAssetName = FString();
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};


	/*
	 * Player Abilities
	  */
USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel;
};
inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

/**
 * 
 */
UCLASS()
class SUSTAINABLEWIZARDRY_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	int32 WaveCount = 0;

	UPROPERTY()
	FName PlayerStartTag;
	
	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FString PlayerName = FString("Default Name");

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstTimeLoadIn = true;
	
	UPROPERTY()
	bool bSettingsSaveFlag = true;
	
	/*
	 * Player Stats
	 */

	// Location
	UPROPERTY()
	FVector PlayerSaveLocation;

	// General
	UPROPERTY()
	int32 PlayerLevel = 1;
	UPROPERTY()
	int32 XP = 0;
	UPROPERTY()
	int32 SpellPoints = 0;

	// Attributes
	UPROPERTY()
	float Deep = 0;
	UPROPERTY()
	float Flare = 0;
	UPROPERTY()
	float Swift = 0;
	UPROPERTY()
	float Seismic = 0;

	/*
	 * Abilities
	 */

	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	/*
	 * Map Saves
	 */
	
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;
	
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	
	bool HasMap(const FString& InMapName);
	
};
