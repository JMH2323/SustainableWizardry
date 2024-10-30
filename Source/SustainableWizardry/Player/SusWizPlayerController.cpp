// Fill out your copyright notice in the Description page of Project Settings.



#include "SusWizPlayerController.h"

#include <string>

#include "AbilitySystemBlueprintLibrary.h"
#include "AttributeSet.h"
#include "SustainableWizardry/GAS/ASC/SusWizAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "SustainableWizardry/Effects/Actors/SolarBeamDecal.h"
#include "SustainableWizardry/UI/Widget/WidgetComponent/DamageTextComponent.h"
#include "SustainableWizardry/Input/SusWizInputComponent.h"


ASusWizPlayerController::ASusWizPlayerController()
{
	bReplicates = true;
}

void ASusWizPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<ASolarBeamDecal>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void ASusWizPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

FVector ASusWizPlayerController::GetMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		return MagicCircle->GetActorLocation();
	}
	else
	{
		FVector Failcase;
		return Failcase;
	}
	
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

bool ASusWizPlayerController::IsInputLeftHanded(FGameplayTag Input)
{
	if (Input.MatchesTagExact(FSusWizGameplayTags::Get().InputTag_LMB)) return true;
	if (Input.MatchesTagExact(FSusWizGameplayTags::Get().InputTag_1)) return true;
	if (Input.MatchesTagExact(FSusWizGameplayTags::Get().InputTag_3)) return true;
	return false;
}


void ASusWizPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	LookAxisVector *= MouseSensitivity;
	
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

	if (IsValid(MagicCircle))
	{
		UpdateMagicCircleLocation();
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
	
	if (GetASC() == nullptr) return;
	

	if (IsInputLeftHanded(InputTag) && GetASC()->HasMatchingGameplayTag(FSusWizGameplayTags::Get().Player_Block_LInputPressed))
	{
		return;
	}
	if (!IsInputLeftHanded(InputTag) &&	GetASC()->HasMatchingGameplayTag(FSusWizGameplayTags::Get().Player_Block_RInputPressed))
	{
		return;
	}
	
	GetASC()->AbilityInputTagPressed(InputTag);
	
}

void ASusWizPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{

	if (GetASC() == nullptr) return;
	
	if (IsInputLeftHanded(InputTag) && GetASC()->HasMatchingGameplayTag(FSusWizGameplayTags::Get().Player_Block_LInputReleased))
	{
		return;
	}
	if (!IsInputLeftHanded(InputTag) && GetASC()->HasMatchingGameplayTag(FSusWizGameplayTags::Get().Player_Block_RInputReleased))
	{
		return;
	}
	
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ASusWizPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

	if (GetASC() == nullptr) return;
	
	if (IsInputLeftHanded(InputTag) && GetASC()->HasMatchingGameplayTag(FSusWizGameplayTags::Get().Player_Block_LInputHeld))
	{
		return;
	}
	if (!IsInputLeftHanded(InputTag) && GetASC()->HasMatchingGameplayTag(FSusWizGameplayTags::Get().Player_Block_RInputHeld))
	{
		return;
	}
	
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

void ASusWizPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{

		/// Get the player's forward vector and pitch to calculate distance of AOE Decal.
		FVector PlayerForwardVector = GetPawn()->GetActorForwardVector();
		FRotator CameraRotation = GetControlRotation();
		float CameraPitch = CameraRotation.Pitch;
		if (CameraPitch > 180.0f)
		{
			CameraPitch -= 360.0f;
		}
		CameraPitch += 15.f;
		
		
        float NormalizedPitch = FMath::Clamp((CameraPitch * 5.0f) / 180.0f, 0.0f, 1.0f);
		float Distance = NormalizedPitch * 7000.0f;
		if(Distance < 500.f) Distance = 500.f;
		
		FVector NewLocation = GetPawn()->GetActorLocation() + FVector(PlayerForwardVector.X, PlayerForwardVector.Y, 0).GetSafeNormal() * Distance;
		NewLocation.Z = GetPawn()->GetActorLocation().Z; 

		/* Debug logs to verify the calculations
		UE_LOG(LogTemp, Warning, TEXT("Player Location: %s"), *GetPawn()->GetActorLocation().ToString());
		UE_LOG(LogTemp, Warning, TEXT("Player Forward Vector: %s"), *PlayerForwardVector.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Camera Pitch: %f"), CameraPitch);
		UE_LOG(LogTemp, Warning, TEXT("Normalized Pitch: %f"), NormalizedPitch);
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
		UE_LOG(LogTemp, Warning, TEXT("New Location: %s"), *NewLocation.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			FString::Printf(TEXT("%f"), CameraPitch));
			*/
		
		MagicCircle->SetActorLocation(NewLocation);
		
	}
}
