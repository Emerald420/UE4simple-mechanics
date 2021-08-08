// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	if (_pawn == nullptr)
	{
		_pawn = TryGetPawnOwner();
		_character = Cast<AMyCharacter>(_pawn);
	}


}

void UMyAnimInstance::UpdateAnimationProperties()
{
	if (_pawn == nullptr)
	{
		_pawn = TryGetPawnOwner();
	}
	if (_pawn)
	{
		FVector speed = _pawn->GetVelocity();
		FVector lateralSpeed = FVector(speed.X, speed.Y, 0.f);
		_movementSpeed = lateralSpeed.Size();
		_bIsInAir = _pawn->GetMovementComponent()->IsFalling();	
		
		if (_character == nullptr)
		{
			_character = Cast<AMyCharacter>(_pawn);
		}
		if (_character)
		{
			_bIsRunningWall = _character->_bisRunningWall;
			_wallRunningRight = _character->_wallRunningDirection == -1.f ? true : false;
			
		}
	}
}

void UMyAnimInstance::AirCheck()
{
	if (_pawn == nullptr)
	{
		_pawn = TryGetPawnOwner();
	}
	if (_pawn)
	{
		_character = Cast<AMyCharacter>(_pawn);
		if (_character)
		{
			if (_bIsInAir)
			{
				_character->_characterMovementComponent->bOrientRotationToMovement = false;
			}
			else
			{
				_character->_characterMovementComponent->bOrientRotationToMovement = true;
			}
		}
	}
}

