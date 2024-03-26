// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizProjectileSpell.h"

#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

void USusWizProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		// Getting the player's viewpoint.
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		ActorInfo->PlayerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		// Calculating the shot direction from the socket location to the player's viewpoint.
		FVector ShotDirection = ActorInfo->PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
		ShotDirection = -PlayerViewPointRotation.Vector();
		
		// Setting this direction as the rotation for the SpawnTransform.
		SpawnTransform.SetRotation(ShotDirection.Rotation().Quaternion());
		
		
		ASusWizProjectiles* Projectile = GetWorld()->SpawnActorDeferred<ASusWizProjectiles>(ProjectileClass, SpawnTransform,
			GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		// TODO: Give Projectile a GE Spec for damage
		Projectile->FinishSpawning(SpawnTransform);
	}

	
	
}
