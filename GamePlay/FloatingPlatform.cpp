// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = _staticMesh;

	_startPoint = FVector(0.f);
	_endPoint = FVector(0.f);

	_interpSpeed = 4.f;
	_interpTime = 1.f;
	_bInterp = false;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	_startPoint = GetActorLocation();
	_endPoint += _startPoint;
	_bInterp = false;

	GetWorldTimerManager().SetTimer(_interpTimer,this,&AFloatingPlatform::ToggleInterping,_interpTime);

	_distance = (_endPoint - _startPoint).Size();

}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (_bInterp)
	{
		FVector currentLocation = GetActorLocation();
		FVector interp = FMath::VInterpTo(currentLocation, _endPoint, DeltaTime, _interpSpeed);
		SetActorLocation(interp);

		float distanceTravled = (GetActorLocation() - _startPoint).Size();
		if (_distance - distanceTravled <= 1.f)
		{
			ToggleInterping();		
			GetWorldTimerManager().SetTimer(_interpTimer, this, &AFloatingPlatform::ToggleInterping, _interpTime);
			SwapVectors(_startPoint, _endPoint);
		}
	}
	
}


void AFloatingPlatform::ToggleInterping()
{
	_bInterp = !_bInterp;
}


void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector temp = VecOne;
	VecOne = VecTwo;
	VecTwo = temp;
}