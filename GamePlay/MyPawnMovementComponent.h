// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API UMyPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE void SetSpeed(float speed) { _maxSpeed = speed; }
	FORCEINLINE float GetSpeed() { return _maxSpeed; }
private:
	float _maxSpeed;

};
