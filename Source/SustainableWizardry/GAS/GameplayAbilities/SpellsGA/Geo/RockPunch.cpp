// Fill out your copyright notice in the Description page of Project Settings.


#include "RockPunch.h"

#include "SustainableWizardry/SustainableWizardry.h"
#include "SustainableWizardry/GAS/SusWizAbilitySystemLibrary.h"
#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"

FString URockPunch::GetDescription(int32 Level)
{
	return Super::GetDescription(Level);
}

FString URockPunch::GetNextLevelDescription(int32 Level)
{
	return Super::GetNextLevelDescription(Level);
}

void URockPunch::FindAndSpawnGeoProjectile()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	// Determine the size of projectile to spawn based on level.
	switch(GetAbilityLevel())
		{
		case 1:
			SpawnGeoProjectile(GeoProjectileOne);
			break;

		case 2:
			SpawnGeoProjectile(GeoProjectileTwo);
			break;

		case 3:
			SpawnGeoProjectile(GeoProjectileThree);
			break;

		case 4:
			SpawnGeoProjectile(GeoProjectileFour);
			break;

		case 5:
			SpawnGeoProjectile(GeoProjectileFive);
			break;

		default:
			SpawnGeoProjectile(GeoProjectileFive);
			break;
		}
	
		
	}

const void URockPunch::SpawnGeoProjectile(TSubclassOf<ASusWizProjectiles> GeoProjectile)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
    if (!bIsServer) return;

    ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
    if (CombatInterface && StoredActorInfo)
    {

    	FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
    	
    	if (isLeftHanded)
    	{
    		SocketLocation = CombatInterface->GetSecCombatSocketLocation();
    	}

    	

    	
        // Getting the player's viewpoint.
        FVector PlayerViewPointLocation;
        FRotator PlayerViewPointRotator;
        StoredActorInfo->PlayerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotator);

    	// Unique: Calculate the rock offset relative to player's view
    	FVector RockOffsetRelative = PlayerViewPointRotator.RotateVector(RockOffset);
    	SocketLocation = SocketLocation - RockOffsetRelative;
    	
    	FTransform SpawnTransform;
        // Get the forward vector of the player viewpoint
        FVector ViewPointForward = PlayerViewPointRotator.Vector();

        // Create a line trace from the viewport location in the direction that the viewport is facing.
        FVector EndPoint = PlayerViewPointLocation + (ViewPointForward * 50000);  // 50000 is your trace distance

        FHitResult TraceResult;
        FCollisionQueryParams TraceParams;
        bool bHit = GetWorld()->LineTraceSingleByChannel(TraceResult, PlayerViewPointLocation, EndPoint, ECC_Visibility, TraceParams);

        // The direction in which the projectile should be shot.
        FVector ShotDirection;

        if(bHit)
        {
            // If the line trace hits something, shoot in the direction of the hit.
            ShotDirection = (TraceResult.ImpactPoint - SocketLocation).GetSafeNormal();
        }
        else
        {
            // If the line trace does not hit anything, shoot in the direction of the line trace end point.
            ShotDirection = (EndPoint - SocketLocation).GetSafeNormal();
        }

        // Set the spawn rotation using the shot direction.
        FRotator Rotation = ShotDirection.Rotation();
    	Rotation.Pitch = PitchOverride;
        SpawnTransform.SetLocation(SocketLocation);
        SpawnTransform.SetRotation(Rotation.Quaternion());

        // Spawn the projectile.
        ASusWizProjectiles* Projectile = GetWorld()->SpawnActorDeferred<ASusWizProjectiles>(GeoProjectile, SpawnTransform,
            GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
        
        Projectile->FinishSpawning(SpawnTransform);
	}

}
