


#include "SusWizRoboProjectile.h"
#include "SustainableWizardry/GAS/GameplayAbilities/Spells/SpellsBase/SusWizProjectiles.h"
#include "SustainableWizardry/Interaction/CombatInterface.h"
#include "SustainableWizardry/SusWizGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


void USusWizRoboProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // No need to store ActorInfo for enemies
}

void USusWizRoboProjectile::SpawnProjectile()
{
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
    if (!bIsServer) return;

    // Get the enemy's location
    FVector EnemyLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

    // Define an offset for spawning the projectile (in front of the enemy)
    FVector SpawnOffset = FVector(100.0f, 0.0f, 0.0f); // Adjust as needed

    // Calculate the spawn location by adding the offset to the enemy's location
    FVector SpawnLocation = EnemyLocation + SpawnOffset;

    // Set the spawn rotation to face the same direction as the enemy
    FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorRotation();

    // Create the transform for spawning the projectile
    FTransform SpawnTransform(SpawnRotation, SpawnLocation);

    // Spawn the projectile
    ASusWizProjectiles* Projectile = GetWorld()->SpawnActorDeferred<ASusWizProjectiles>(ProjectileClass, SpawnTransform,
                                                                                          GetOwningActorFromActorInfo(),
                                                                                          Cast<APawn>(GetOwningActorFromActorInfo()),
                                                                                          ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    if (!Projectile) return; // Check if the spawned projectile is valid

    // Give the projectile a GE Spec for damage.
    const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
    if (!SourceASC) return; // Check if the ability system component is valid

    const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

    if (!SpecHandle.IsValid()) return; // Check if the effect spec handle is valid

    /* Damage from Tags Meta */
    FSusWizGameplayTags GameplayTags = FSusWizGameplayTags::Get();
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 50.f);
    const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);

    Projectile->DamageEffectSpecHandle = SpecHandle;

    Projectile->FinishSpawning(SpawnTransform);
}
