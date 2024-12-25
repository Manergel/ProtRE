#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

// Constructor
AMyCharacter::AMyCharacter()
{
	// Enable Tick
	PrimaryActorTick.bCanEverTick = true;

	Speed = 0.0f;

	// Create and setup Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	// Create and setup Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Initialize Camera and Aiming settings
	DefaultFOV = 90.0f;
	AimingFOV = 70.0f;
	CameraTransitionSpeed = 10.0f;
	bIsAiming = false;
	AmmoCount = 10;
}

// BeginPlay
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set initial FOV
	Camera->SetFieldOfView(DefaultFOV);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Tick
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smoothly transition camera FOV
	float TargetFOV = bIsAiming ? AimingFOV : DefaultFOV;
	float CurrentFOV = Camera->FieldOfView;
	float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, CameraTransitionSpeed);
	Camera->SetFieldOfView(NewFOV);
}

// Move
void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

// Look
void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Start Aiming
void AMyCharacter::StartAiming()
{
	bIsAiming = true;
}

// Stop Aiming
void AMyCharacter::StopAiming()
{
	bIsAiming = false;
}

// SetupPlayerInputComponent
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMyCharacter::Shoot);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyCharacter::Reload);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

		// Bind Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		// Bind Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMyCharacter::StartAiming);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMyCharacter::StopAiming);
	}
}

void AMyCharacter::Shoot()
{
	if (AmmoCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of Ammo"));
		return;
	}

	AmmoCount--;

	if (MuzzleFlashEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, GetActorLocation());
	}

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * 10000.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		if (ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location);
		}

		if (AActor* HitActor = HitResult.GetActor())
		{
			UGameplayStatics::ApplyDamage(HitActor, 10.0f, GetController(), this, nullptr);
			UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
		}
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
}

void AMyCharacter::Reload()
{
	AmmoCount = 10;
	UE_LOG(LogTemp, Log, TEXT("Reloaded! Ammo Count: %d"), AmmoCount);
}



