// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = Animations)
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable,Category = Animations)
	void AirCheck();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float _movementSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category =Movement)
	bool _bIsInAir;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool _bIsRunningWall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool _wallRunningRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMyCharacter* _character;
	
	class APawn* _pawn;
};
