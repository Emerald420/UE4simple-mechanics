// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:


	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = Animations)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Movement)
	float _movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* _pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AEnemy* _character;
};
