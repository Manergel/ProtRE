#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PistolWeapon.generated.h"

class UParticleSystem;
class USoundBase;

UCLASS()
class PROTRE_API APistolWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    APistolWeapon();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Fire the weapon
    void Fire();

    // Reload the weapon
    void Reload();

    // Get the current ammo count (useful for UI)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    int32 GetAmmoCount() const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // Ammo properties
    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 MaxAmmo;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 CurrentAmmo;

    // Shooting properties
    UPROPERTY(EditAnywhere, Category = "Weapon")
    float FireRange;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    float Damage;

    // Visual and audio effects
    UPROPERTY(EditAnywhere, Category = "Weapon")
    UParticleSystem* MuzzleFlash;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    UParticleSystem* ImpactEffect;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    USoundBase* FireSound;

    // Raycasting functionality
    void PerformRaycast();
};

