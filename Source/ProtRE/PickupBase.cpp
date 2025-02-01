#include "PickupBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Constructor
APickupBase::APickupBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create Mesh
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    RootComponent = PickupMesh;

    // Create Collision Sphere
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetupAttachment(RootComponent);
    CollisionSphere->SetSphereRadius(100.f);

    // Enable Overlap Events
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnPickup);
}

// Begin Play
void APickupBase::BeginPlay()
{
    Super::BeginPlay();
}

// Tick (Not needed here)
void APickupBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Pickup Logic
void APickupBase::OnPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
    if (PlayerCharacter)
    {
        switch (PickupType)
        {
        case EPickupType::Weapon:
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Picked up a Weapon!"));
            break;

        case EPickupType::Ammo:
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Picked up %d Ammo!"), PickupValue));
            break;

        case EPickupType::Health:
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Restored %d Health!"), PickupValue));
            break;
        }

        Destroy(); // Remove the pickup from the world
    }
}



