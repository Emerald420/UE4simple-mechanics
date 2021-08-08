// Fill out your copyright notice in the Description page of Project Settings.


#include "Items.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AItems::AItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_collisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = _collisionVolume;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	_mesh->SetupAttachment(GetRootComponent());

	_idleParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	_idleParticlesComponent->SetupAttachment(GetRootComponent());

	_bRotate = false;
	_rotatingRate = 45.f;
}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();
	_collisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnOverlapBegin);
	_collisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItems::OnOverlapEnd);

}

// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_bRotate)
	{
		FRotator newRotation = GetActorRotation();
		newRotation.Yaw += DeltaTime * _rotatingRate;
		SetActorRotation(newRotation);
	}

}

void AItems::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AItems::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}