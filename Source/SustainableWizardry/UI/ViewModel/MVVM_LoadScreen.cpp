// Fill out your copyright notice in the Description page of Project Settings.


#include "MVVM_LoadScreen.h"
#include "MVVM_LoadSlot.h"
#include "SustainableWizardry/Game/SuzWizGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/Game/Instance/SusWizGameInstance.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
	
	LoadSlot_3 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_3->LoadSlotName = FString("LoadSlot_3");
	LoadSlot_3->SlotIndex = 3;
	LoadSlots.Add(3, LoadSlot_3);
}

void UMVVM_LoadScreen::OnSlotSelected()
{
	UE_LOG(LogTemp, Warning, TEXT("Slot has been selected"));
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
		return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(this));
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SetWaveCount(0);
	LoadSlots[Slot]->SetMapName(SusWizGameMode->DefaultMapName);
	LoadSlots[Slot]->SetPlayerLevel(1);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->PlayerStartTag = SusWizGameMode->DefaultPlayerStartTag;
	
	SusWizGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();

	USusWizGameInstance* SusWizGameInstance = Cast<USusWizGameInstance>(SusWizGameMode->GetGameInstance());
	SusWizGameInstance->LoadSlotName = LoadSlots[Slot]->LoadSlotName;
	SusWizGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	SusWizGameInstance->PlayerStartTag = SusWizGameMode->DefaultPlayerStartTag;
	
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		ASuzWizGameModeBase::DeleteSlot(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>
	(UGameplayStatics::GetGameMode(this));

	USusWizGameInstance* SusWizGameInstance = Cast<USusWizGameInstance>(SusWizGameMode->GetGameInstance());
	SusWizGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	SusWizGameInstance->LoadSlotName = SelectedSlot->LoadSlotName;
	SusWizGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	if (IsValid(SelectedSlot))
	{
		SusWizGameMode->TravelToMap(SelectedSlot);	
	}
}

void UMVVM_LoadScreen::LoadData()
{
	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = SusWizGameMode->GetSaveSlotData(LoadSlot.Value->LoadSlotName, LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
		
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SetWaveCount(SaveObject->WaveCount);
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
		LoadSlot.Value->InitializeSlot();
	}
}

UMVVM_LoadScreen::UMVVM_LoadScreen()
{
	// Ensure to bind the delegate in the constructor
	SlotSelected.AddDynamic(this, &UMVVM_LoadScreen::OnSlotSelected);
}
