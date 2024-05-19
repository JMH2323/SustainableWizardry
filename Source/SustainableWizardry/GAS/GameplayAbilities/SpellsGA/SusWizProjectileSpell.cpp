// Fill out your copyright notice in the Description page of Project Settings.


#include "SusWizProjectileSpell.h"

#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void USusWizProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Storing ActorInfo for future use
	StoredActorInfo = ActorInfo;
	
}

void USusWizProjectileSpell::SpawnProjectile()
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
          	
        FTransform SpawnTransform;

    	
        // Getting the player's viewpoint.
        FVector PlayerViewPointLocation;
        FRotator PlayerViewPointRotator;
        StoredActorInfo->PlayerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotator);

        // Get the forward vector of the player viewpoint
        FVector ViewPointForward = PlayerViewPointRotator.Vector();

        // Create a line trace from the viewport location in the direction that the viewport is facing.
        FVector EndPoint = PlayerViewPointLocation + (ViewPointForward * 5000);  // 5000 is your trace distance

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
        SpawnTransform.SetLocation(SocketLocation);
        SpawnTransform.SetRotation(Rotation.Quaternion());

        // Spawn the projectile.
        ASusWizProjectiles* Projectile = GetWorld()->SpawnActorDeferred<ASusWizProjectiles>(ProjectileClass, SpawnTransform,
            GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// Give Projectile a GE Spec for damage.
    	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
    	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());


    	/* Damage from Tags Meta */
    	FSusWizGameplayTags GameplayTags = FSusWizGameplayTags::Get();
    	

    	for (auto& Pair : DamageTypes)
    	{
    		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
    		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
    	}

    	

    	Projectile->DamageEffectSpecHandle = SpecHandle;
        
        Projectile->FinishSpawning(SpawnTransform);
	}
	
}
