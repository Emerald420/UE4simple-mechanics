// Fill out your copyright notice in the Description page of Project Settings.


#include "NewFloater.h"


// Sets default values
ANewFloater::ANewFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("customStaticMesh"));

	_initialLocation = FVector(0.0f);

	_placeLocation = FVector(0.0f);

	_worldOrigin = FVector(0.0f, 0.0f, 0.0f);

	_InitialDirection = FVector(0.0f, 0.0f, 0.0f);

	_initialForce = FVector(0.0f, 0.0f, 0.0f);

	_initalTorque = FVector(0.0f, 0.0f, 0.0f);

	_bInitializerFloaterLocation = true;

	_bShouldFloat = true;


}

// Called when the game starts or when spawned
void ANewFloater::BeginPlay()
{
	Super::BeginPlay();
	
	_placeLocation = GetActorLocation();

	if (_bInitializerFloaterLocation)
	{
		SetActorLocation(_initialLocation);
	}

}

// Called every frame
void ANewFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_bShouldFloat)
	{
		FHitResult HitResult;
		AddActorLocalOffset(_InitialDirection, true, &HitResult);
	
		FVector HitLocation = HitResult.Location;

		UE_LOG(LogTemp, Warning, TEXT("HIT Location: x = %f, y = %f, z = %f"),HitLocation.X, HitLocation.Y, HitLocation.Z);
	
	}

	/*_randomVector = { FMath::FRand(),FMath::FRand(),FMath::FRand()};
	VECTORLOG(_randomVector.X, _randomVector.Y, _randomVector.Z );*/


	//FRotator rotation = GetActorRotation();
	//FRotator xRot = {0,0,50.f};
	//AddActorLocalRotation(xRot  * DeltaTime);

	//_staticMesh->AddForce(_initialForce);
	//VECTORLOG(_initialForce.X,_initialForce.Y,_initialForce.Z);

	//_staticMesh->AddTorqueInDegrees(_initalTorque);
	
	
}



