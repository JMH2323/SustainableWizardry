// Fill out your copyright notice in the Description page of Project Settings.



#include "SusWizPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AttributeSet.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "SustainableWizardry/UI/Widget/WidgetComponent/DamageTextComponent.h"
#include "SustainableWizardry/Input/SusWizInputComponent.h"


ASusWizPlayerController::ASusWizPlayerController()
{
	bReplicates = true;
}

void ASusWizPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bDodgedHit, bool bCrit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bDodgedHit, bCrit);
	}
}

void ASusWizPlayerController::BeginPlay()
{
	
	Super::BeginPlay();

	// Initialize Subsystem for Enhanced Input.
	check(SusWizContext);
    
	if(const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(SusWizContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to get EnhancedInputLocalPlayerSubsystem."));
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is not of type ULocalPlayer or is nullptr."));
	}

	
}

void ASusWizPlayerController::SetupInputComponent()
{
	
	Super::SetupInputComponent();
	
	USusWizInputComponent* SusWizInputComponent = CastChecked<USusWizInputComponent>(InputComponent);
	
	///Moving
	SusWizInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASusWizPlayerController::Move);
	
	//Looking
	SusWizInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASusWizPlayerController::Look);

	// Ability Actions
	SusWizInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
}


void ASusWizPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Inverting Y Axis
	LookAxisVector.Y *= -1;

	// Warning log for checking mouse input
	//UE_LOG(LogTemp, Warning, TEXT("Checking Axes: X: %f, Y: %f"), LookAxisVector.X, LookAxisVector.Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->bUseControllerRotationPitch = true;
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
		
	
}

void ASusWizPlayerController::Move(const FInputActionValue& InputActionValue)
{

	// input is a Vector2D
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// Get the Actor info from the controller's perspective
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Add movement.
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}

	
}

void ASusWizPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//
}

void ASusWizPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ASusWizPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

USusWizAbilitySystemComponent* ASusWizPlayerController::GetASC()
{
	if (SusWizAbilitySystemComponent == nullptr)
	{
		SusWizAbilitySystemComponent = Cast<USusWizAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return SusWizAbilitySystemComponent;
}
