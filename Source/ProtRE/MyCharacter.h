#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UEnhancedInputComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PROTRE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Constructor
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Enhanced Input Actions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartAiming();
	void StopAiming();

	// Input System Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AimAction;

	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	// Camera and Aiming Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DefaultFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float AimingFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraTransitionSpeed;

	// Combat logic
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Combat")
	int32 AmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UParticleSystem* MuzzleFlashEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UParticleSystem* ImpactEffect;

	void Shoot();
	void Reload();

	bool bIsAiming;

	// Setup player input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float Speed;

	void UpdateAnimationSpeed();
};


