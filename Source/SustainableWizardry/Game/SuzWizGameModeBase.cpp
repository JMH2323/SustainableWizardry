// Fill out your copyright notice in the Description page of Project Settings.


#include "SuzWizGameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Instance/SusWizGameInstance.h"
#include "SustainableWizardry/Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SustainableWizardry/SusWizLogChannels.h"
#include "SustainableWizardry/Interaction/SaveInterface.h"
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

void ASuzWizGameModeBase::SaveWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	USusWizGameInstance* SusWizGI = Cast<USusWizGameInstance>(GetGameInstance());
	check(SusWizGI)

	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(SusWizGI->LoadSlotName, SusWizGI->LoadSlotIndex))
	{
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty(); // We will fill with "Fsaved Actors"

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if(!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(SavedActor.Bytes);

			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;

			Actor->Serialize(Archive);

			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, SusWizGI->LoadSlotName, SusWizGI->LoadSlotIndex);
		
	}
	
}

void ASuzWizGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	USusWizGameInstance* SusWizGI = Cast<USusWizGameInstance>(GetGameInstance());
	check(SusWizGI)

	if (UGameplayStatics::DoesSaveGameExist(SusWizGI->LoadSlotName, SusWizGI->LoadSlotIndex))
	{

		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(SusWizGI->LoadSlotName, SusWizGI->LoadSlotIndex));
		if (SaveGame ==  nullptr)
		{
			UE_LOG(LogSusWiz, Error, TEXT("Failed to load save slot"))
			return;
		}
		
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive); // converts binary bytes back into variables
					
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
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
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;
	SetWaveCount(0);
	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapName));
}

void ASuzWizGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
