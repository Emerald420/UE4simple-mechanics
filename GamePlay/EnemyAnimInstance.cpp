// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (_pawn == nullptr)
	{
		_pawn = TryGetPawnOwner();
		if (_pawn)
		{
			_character = Cast<AEnemy>(_pawn);
		}

	}

}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (_pawn == nullptr)
	{
		_pawn = TryGetPawnOwner();
		if (_pawn)
		{
			_character = Cast<AEnemy>(_pawn);
		}	
	}
	if (_pawn)
	{
		FVector speed = _pawn->GetVelocity();
		FVector lateralSpeed = FVector(speed.X, speed.Y, 0.f);
		_movementSpeed = lateralSpeed.Size();
	}
}

