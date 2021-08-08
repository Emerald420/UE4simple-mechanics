// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalmeshComponent.h"
#include "Components/InputComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")); // already declared in AActor

	_meshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	_meshComponent->SetupAttachment(GetRootComponent());
	
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_camera->SetupAttachment(GetRootComponent());
	_camera->SetRelativeLocation(FVector(-300.f,0.f,300.f));
	_camera->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	_currentVelocity = FVector(0.0f);
	_maxSpeed = 100.f;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation()+ _currentVelocity *DeltaTime;
	SetActorLocation(newLocation);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPawn::MoveRight);


}

void AMyPawn::MoveForward(float value)
{
	_currentVelocity.Y = FMath::Clamp(value,-1.f,1.f) *_maxSpeed;
}

void AMyPawn::MoveRight(float value)
{
	_currentVelocity.X = FMath::Clamp(value, -1.f, 1.f) * _maxSpeed;
}