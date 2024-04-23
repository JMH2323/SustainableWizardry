// Fill out your copyright notice in the Description page of Project Settings.



#include "SusWizEnemy.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "SustainableWizardry/GAS/Attribute/SusWizAttributeSet.h"
#include "SustainableWizardry/UI/Widget/UserWidget/SusWizUserWidget.h"
#include "Components/WidgetComponent.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SustainableWizardry/AI/Controller/SusWizAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"

ASusWizEnemy::ASusWizEnemy()
{

	AbilitySystemComponent = CreateDefaultSubobject<USusWizAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USusWizAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
}

void ASusWizEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	SusWizAIController = Cast<ASusWizAIController>(NewController);
	SusWizAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	SusWizAIController->RunBehaviorTree(BehaviorTree);
	SusWizAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	SusWizAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	
	
	
}

int32 ASusWizEnemy::GetPlayerLevel()
{
	return Level;
}


void ASusWizEnemy::Die()
{
	SetLifeSpan(Lifespan);
	Super::Die();
}

void ASusWizEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ASusWizEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}


void ASusWizEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		USusWizAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	
	if (USusWizUserWidget* SusWizUserWidget = Cast<USusWizUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		SusWizUserWidget->SetWidgetController(this);
	}

	if (const USusWizAttributeSet* SusWizAS = Cast<USusWizAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SusWizAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SusWizAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);


		AbilitySystemComponent->RegisterGameplayTagEvent(FSusWizGameplayTags::Get().Effects_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(
				this, &ASusWizEnemy::HitReactTagChanged);
		

		OnHealthChanged.Broadcast(SusWizAS->GetHealth());
		OnMaxHealthChanged.Broadcast(SusWizAS->GetMaxHealth());
	}
	
}


void ASusWizEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	SusWizAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}



void ASusWizEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<USusWizAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
	
}

void ASusWizEnemy::InitializeDefaultAttributes() const
{
	USusWizAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}