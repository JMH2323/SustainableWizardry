// Fill out your copyright notice in the Description page of Project Settings.


#include "SuzWizGameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "Instance/SusWizGameInstance.h"
#include "SustainableWizardry/Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/UI/ViewModel/MVVM_LoadSlot.h"

void ASuzWizGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->WaveCount = LoadSlot->GetWaveCount();
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
}

ULoadScreenSaveGame* ASuzWizGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
	
}

void ASuzWizGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* ASuzWizGameModeBase::RetrieveInGameSaveData()
{
	USusWizGameInstance* SusWizGameInstance = Cast<USusWizGameInstance>(GetGameInstance());
	const FString InGameLoadSlotName = SusWizGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = SusWizGameInstance->LoadSlotIndex;
	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void ASuzWizGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	USusWizGameInstance* SusWizGameInstance = Cast<USusWizGameInstance>(GetGameInstance());
	const FString InGameLoadSlotName = SusWizGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = SusWizGameInstance->LoadSlotIndex;
	SusWizGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;
	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void ASuzWizGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->LoadSlotName;
	const int32 SlotIndex = Slot->SlotIndex;
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* ASuzWizGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{

	USusWizGameInstance* SusWizGameInstance = Cast<USusWizGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == SusWizGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void ASuzWizGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	//
}

void ASuzWizGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
