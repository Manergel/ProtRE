// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

UCLASS()
class PROTRE_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float Speed;

protected:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};