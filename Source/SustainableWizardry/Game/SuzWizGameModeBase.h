// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SuzWizGameModeBase.generated.h"


/**
 * 
 */
class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;
UCLASS()
class SUSTAINABLEWIZARDRY_API ASuzWizGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	ULoadScreenSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	void SaveWorldState(UWorld* World) const;
	void LoadWorldState(UWorld* World) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SusWizWaveCount;

	UFUNCTION(BlueprintCallable)
	int32 GetWaveCount(){return SusWizWaveCount; };
	UFUNCTION(BlueprintCallable)
	void SetWaveCount(int32 NewWaveCount){ SusWizWaveCount = NewWaveCount; };
	
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	void TravelToMap(UMVVM_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	void PlayerDied(ACharacter* DeadCharacter);
	UFUNCTION(BlueprintImplementableEvent)
	void DeathScreen();
	
protected:
	virtual void BeginPlay() override;
	
};
