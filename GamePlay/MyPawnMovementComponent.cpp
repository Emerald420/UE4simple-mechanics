// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawnMovementComponent.h"

void UMyPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType,  ThisTickFunction);
	
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.f) *DeltaTime * _maxSpeed;

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult HitResult;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(),true,HitResult);
	
		if (HitResult.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f -HitResult.Time,HitResult.Normal,HitResult);
		}

	}
}
