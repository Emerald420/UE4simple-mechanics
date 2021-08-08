// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyPawnMovementComponent.h"


// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(_sphereComponent);
	_sphereComponent->InitSphereRadius(40.f);
	_sphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	_meshComponent->SetupAttachment(GetRootComponent());

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArm->SetupAttachment(GetRootComponent());
	_springArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	_springArm->TargetArmLength = 400.f;
	_springArm->bEnableCameraLag = true;
	_springArm->CameraLagSpeed = 3.f;
	


	_camera->SetupAttachment(_springArm,USpringArmComponent::SocketName);
	
	_maxSpeed = 300.f;
	_movementComponent = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("MyMovementComponent"));
	_movementComponent->UpdatedComponent = RootComponent;
	_movementComponent->SetSpeed(_maxSpeed);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sMeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (sMeshComponentAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Success!"));
		_meshComponent->SetStaticMesh(sMeshComponentAsset.Object);
		_meshComponent->SetRelativeLocation(FVector(0.f,0.f,-40.f));
		_meshComponent->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	}	

	_cameraInput = FVector2D(0.f,0.f);


	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	FRotator newRotation = GetActorRotation();

	newRotation.Yaw += _cameraInput.X;
	SetActorRotation(newRotation);

	FRotator newSpringArmRotation = _springArm->GetComponentRotation();
	newSpringArmRotation.Pitch = FMath::Clamp(newSpringArmRotation.Pitch += _cameraInput.Y, -80.f, -15.f);
	UE_LOG(LogTemp,Warning, TEXT("rotation x,y = %f,%f"),newSpringArmRotation.Yaw, newSpringArmRotation.Pitch);
	_springArm->SetWorldRotation(newSpringArmRotation);


}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ACollidingPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&ACollidingPawn::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollidingPawn::CameraPitch);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollidingPawn::CameraYaw);
}

void ACollidingPawn::MoveForward(float value)
{
	FVector Forward = GetActorForwardVector();
	if (_movementComponent)
	{
		_movementComponent->AddInputVector(value * Forward);
	}

}

void ACollidingPawn::MoveRight(float value)
{
	FVector Right = GetActorRightVector();
	if (_movementComponent)
	{
		_movementComponent->AddInputVector(value * Right);
	}
}

void ACollidingPawn::CameraPitch(float value)
{
	_cameraInput.Y = value;
}

void ACollidingPawn::CameraYaw(float value)
{
	_cameraInput.X = value;
}

UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return _movementComponent;
}