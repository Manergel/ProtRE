#include "PistolWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Constructor
APistolWeapon::APistolWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxAmmo = 12; // Default magazine size
    CurrentAmmo = MaxAmmo;

    FireRange = 10000.0f; // 10,000 Unreal units (100 meters)
    Damage = 20.0f; // Damage per shot
}

// BeginPlay
void APistolWeapon::BeginPlay()
{
    Super::BeginPlay();
}

// Tick
void APistolWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Fire
void APistolWeapon::Fire()
{
    if (CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo!"));
        return;
    }

    CurrentAmmo--;

    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GetRootComponent());
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    PerformRaycast();
}

// Perform Raycast
void APistolWeapon::PerformRaycast()
{
    FVector Start = GetActorLocation();
    FVector End = Start + (GetActorForwardVector() * FireRange);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore the weapon itself

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        // Spawn impact effect at the hit location
        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location);
        }

        // Apply damage to the hit actor
        if (AActor* HitActor = HitResult.GetActor())
        {
            UGameplayStatics::ApplyDamage(HitActor, Damage, nullptr, this, nullptr);
            UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
        }
    }

    // Optional: Draw a debug line for testing
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
}

// Reload
void APistolWeapon::Reload()
{
    CurrentAmmo = MaxAmmo;
    UE_LOG(LogTemp, Log, TEXT("Reloaded! Current Ammo: %d"), CurrentAmmo);
}

// Get Ammo Count
int32 APistolWeapon::GetAmmoCount() const
{
    return CurrentAmmo;
}


