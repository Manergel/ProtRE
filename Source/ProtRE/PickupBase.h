#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EPickupType : uint8
{
    Weapon,
    Ammo,
    Health
};

UCLASS()
class PROTRE_API APickupBase : public AActor
{
    GENERATED_BODY()

public:
    APickupBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Pickup")
    EPickupType PickupType; // Type of item (Weapon, Ammo, Health)

    UPROPERTY(EditAnywhere, Category = "Pickup")
    int32 PickupValue; // Amount of ammo/health given (ignored for weapons)

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* PickupMesh; // Mesh for item

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* CollisionSphere; // Collision for detection

    UFUNCTION()
    void OnPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};

