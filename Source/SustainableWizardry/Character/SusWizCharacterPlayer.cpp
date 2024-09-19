// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizCharacterPlayer.h"

#include "SustainableWizardry/Player/SusWizPlayerController.h"
#include "SustainableWizardry/Player/PlayerState/SusWizPlayerState.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/Character/SusWizCharacterPlayer.h"
#include "SustainableWizardry/GAS/Data/LevelUpInfo.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SustainableWizardry/Game/LoadScreenSaveGame.h"
#include "SustainableWizardry/Game/SuzWizGameModeBase.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "SustainableWizardry/GAS/Data/AbilityInfo.h"
#include "SustainableWizardry/GAS/Data/CharacterClassInfo.h"
#include "SustainableWizardry/UI/HUD/SusWizHUD.h"


ASusWizCharacterPlayer::ASusWizCharacterPlayer()
{
	CharacterClass = ECharacterClass::Wizard;
	

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void ASusWizCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server and HUD
	// If this actor is possessed by a player, we access the player's abilities
	InitAbilityActorInfo();
	LoadProgress();
	
}

void ASusWizCharacterPlayer::LoadProgress()
{
	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (SusWizGameMode)
	{
		ULoadScreenSaveGame* SaveData = SusWizGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		

		if(SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			if(ASusWizPlayerState* SusWizPlayerState = Cast<ASusWizPlayerState>(GetPlayerState()))
			{
				SusWizPlayerState->SetLevel(SaveData->PlayerLevel);
				SusWizPlayerState->SetXP(SaveData->XP);
				SusWizPlayerState->SetSpellPoints(SaveData->SpellPoints);
				SusWizPlayerState->SetLocation(SaveData->PlayerSaveLocation);
			}
			
			if (USusWizAbilitySystemComponent* SusWizASC = Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent))
			{
				SusWizASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}

			
			USusWizAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
			
		}
		
	}
}

void ASusWizCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init info for the client
	InitAbilityActorInfo();
	
	
}

int32 ASusWizCharacterPlayer::GetAttributePoints_Implementation() const
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->GetAttributePoints();
}

int32 ASusWizCharacterPlayer::GetSpellPoints_Implementation() const
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->GetSpellPoints();
}

void ASusWizCharacterPlayer::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		SusWizPlayerController->ShowMagicCircle(DecalMaterial);
	}
	
}

void ASusWizCharacterPlayer::HideMagicCircle_Implementation()
{
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		SusWizPlayerController->HideMagicCircle();
	}
}

FVector ASusWizCharacterPlayer::GetMagicCircleLocation_Implementation()
{
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		return SusWizPlayerController->GetMagicCircleLocation();
	}
	else
	{
		FVector Failcase;
		return Failcase;
	}
}

void ASusWizCharacterPlayer::AddToXP_Implementation(int32 InXP)
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	SusWizPlayerState->AddToXP(InXP);
}

void ASusWizCharacterPlayer::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void ASusWizCharacterPlayer::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 ASusWizCharacterPlayer::GetXP_Implementation() const
{
	const ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->GetXP();
}

int32 ASusWizCharacterPlayer::FindLevelForXP_Implementation(int32 InXP) const
{
	const ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ASusWizCharacterPlayer::GetAttributePointsReward_Implementation(int32 Level) const
{
	const ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 ASusWizCharacterPlayer::GetSpellPointsReward_Implementation(int32 Level) const
{
	const ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	return SusWizPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void ASusWizCharacterPlayer::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	SusWizPlayerState->AddToLevel(InPlayerLevel);
	if (USusWizAbilitySystemComponent* SusWizASC = Cast<USusWizAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		SusWizASC->UpdateAbilityStatuses(SusWizPlayerState->GetPlayerLevel());
	}
	
}

void ASusWizCharacterPlayer::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	SusWizPlayerState->AddToAttributePoints(InAttributePoints);
}

void ASusWizCharacterPlayer::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState);
	SusWizPlayerState->AddToSpellPoints(InSpellPoints);
}

void ASusWizCharacterPlayer::SaveProgress_Implementation(const FName& CheckpointTag)
{
	ASuzWizGameModeBase* SusWizGameMode = Cast<ASuzWizGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (SusWizGameMode)
	{
		ULoadScreenSaveGame* SaveData = SusWizGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		

		if(ASusWizPlayerState* SusWizPlayerState = Cast<ASusWizPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = SusWizPlayerState->GetPlayerLevel();
			SaveData->XP = SusWizPlayerState->GetXP();
			SaveData->SpellPoints = SusWizPlayerState->GetSpellPoints();
			SaveData->PlayerSaveLocation = GetActorLocation();
		}
		SaveData->Deep = USusWizAttributeSet::GetDeepAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Flare = USusWizAttributeSet::GetFlareAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Swift = USusWizAttributeSet::GetSwiftAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Seismic = USusWizAttributeSet::GetSeismicAttribute().GetNumericValue(GetAttributeSet());

		SaveData->bFirstTimeLoadIn = false;
		if (!HasAuthority()) return;

		USusWizAbilitySystemComponent* SusWizASC = Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		
		SaveAbilityDelegate.BindLambda([this, SusWizASC, SaveData] (const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = SusWizASC->GetAbilityTagFromSpec(AbilitySpec);
			UAbilityInfo* AbilityInfo = USusWizAbilitySystemLibrary::GetAbilityInfo(this);
			FSusWizAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbility = Info.Ability;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlot = SusWizASC->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = SusWizASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityTypeTag;

			SaveData->SavedAbilities.AddUnique(SavedAbility);
		});
		SusWizASC->ForEachAbility(SaveAbilityDelegate);
		
	
		SusWizGameMode->SaveInGameProgressData(SaveData);
	}
}

int32 ASusWizCharacterPlayer::GetPlayerLevel_Implementation()
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	check(SusWizPlayerState)
	return SusWizPlayerState->GetPlayerLevel();
}

void ASusWizCharacterPlayer::InitAbilityActorInfo()
{
	ASusWizPlayerState* SusWizPlayerState = GetPlayerState<ASusWizPlayerState>();
	

	if(SusWizPlayerState && SusWizPlayerState->GetAbilitySystemComponent())
	{
		//UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is valid"));

		SusWizPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(SusWizPlayerState, this);
		// 6.1
		Cast<USusWizAbilitySystemComponent>(SusWizPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
		
		AbilitySystemComponent = SusWizPlayerState->GetAbilitySystemComponent();
		AttributeSet = SusWizPlayerState->GetAttributeSet();

		// We can apply in c++ but I choose to apply with gameplay effects.
		//AddCharacterAbilities(); // Adding abilities after successful initialization

		//UE_LOG(LogTemp, Warning, TEXT("%s: InitAbilityActorInfo called. AttributeSet: %s"), *GetName(), AttributeSet ? *AttributeSet->GetName() : TEXT("null"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is NOT valid"));
		// This can happen.
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASusWizCharacterPlayer::InitAbilityActorInfo);
		
	}

	// HUD Stuff
	// Tying in the player into the widgets
	if (ASusWizPlayerController* SusWizPlayerController = Cast<ASusWizPlayerController>(GetController()))
	{
		if (ASusWizHUD* SusWizHUD = Cast<ASusWizHUD>(SusWizPlayerController->GetHUD()))
		{
			SusWizHUD->InitOverlay(SusWizPlayerController, SusWizPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}


	// 11.2 initializing default primary attributes
	// InitializePrimaryAttributes();
	// 12.4 Initialize all attributes
	//InitializeDefaultAttributes();

	
};

