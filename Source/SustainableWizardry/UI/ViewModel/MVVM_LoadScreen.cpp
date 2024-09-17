// Fill out your copyright notice in the Description page of Project Settings.


#include "MVVM_LoadScreen.h"
#include "MVVM_LoadSlot.h"
#include "SustainableWizardry/Game/SuzWizGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlots.Add(1, LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlots.Add(2, LoadSlot_2);
	
	LoadSlot_3 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_3->LoadSlotName = FString("LoadSlot_3");
	LoadSlots.Add(3, LoadSlot_3);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
		return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(this));
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SlotStatus = Taken;
	
	SusWizGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();
	
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	
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
		LoadSlot.Value->InitializeSlot();
	}
}
