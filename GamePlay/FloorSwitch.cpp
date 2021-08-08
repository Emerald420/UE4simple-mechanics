// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = _triggerBox;

	_triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	_triggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	_triggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	_triggerBox->SetBoxExtent(FVector(62.f,62.f,32.f));

	_floorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	_floorSwitch->SetupAttachment(GetRootComponent());

	_door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	_door->SetupAttachment(GetRootComponent());

	_switchTime = 2.f;
	_bCharacterOnSwitch = false;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	_triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	_triggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	_initialDoorLocation = _door->GetComponentLocation();
	_initialSwitchLocation = _floorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapBegin working!"));
	if (!_bCharacterOnSwitch)
	{
		_bCharacterOnSwitch = true;
	}
	RaiseDoor();
	LowerFloorSwitch();
}
	
void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapEnd working!"));
	if (_bCharacterOnSwitch)
	{
		_bCharacterOnSwitch = false;
	}
	GetWorldTimerManager().SetTimer(_switchHandle, this, &AFloorSwitch::CloseDoor, _switchTime);
	
}


void AFloorSwitch::UpdateDoorLocation(float z)
{
	FVector newLocation = _initialDoorLocation;
	newLocation.Z += z;
	_door->SetWorldLocation(newLocation);
}

void AFloorSwitch::UpdateFloorSwitchLocation(float z)
{
	FVector newLocation = _initialSwitchLocation;
	newLocation.Z += z;
	_floorSwitch->SetWorldLocation(newLocation);
}

void AFloorSwitch::CloseDoor()
{
	if (!_bCharacterOnSwitch)
	{
		LowerDoor();
		RaiseFloorSwitch();
	}
}