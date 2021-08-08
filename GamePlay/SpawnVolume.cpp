// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/kismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIController.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_spawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	if (_actor1 && _actor2 && _actor3 && _actor4)
	{
		_spawnArray.Add(_actor1);
		_spawnArray.Add(_actor2);
		_spawnArray.Add(_actor3);
		_spawnArray.Add(_actor4);

	}
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector extent = _spawningBox->GetScaledBoxExtent();
	FVector origin = _spawningBox->GetComponentLocation();
	
	FVector point = UKismetMathLibrary::RandomPointInBoundingBox(origin, extent);

	return point;
}

void ASpawnVolume::SpawnOurActor_Implementation(UClass* toSpawn, const FVector& location)
{
	if (toSpawn)
	{
		UWorld* world = GetWorld();
		FActorSpawnParameters spawnParams;

		if (world)
		{
			
			AActor* myPawnSpawned = world->SpawnActor<AActor>(toSpawn,location,FRotator(0.f),spawnParams);		
			AEnemy* enemy = Cast<AEnemy>(myPawnSpawned);
			if (enemy)
			{
				enemy->SpawnDefaultController();
				AAIController* AICon = Cast<AAIController>(enemy->GetController());
				if (AICon)
				{
					enemy->_AIController = AICon;
				}
			}
		}
	}

}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if (_spawnArray.Num() != 0)
	{
		int32 selection = FMath::RandRange(0, _spawnArray.Num() - 1);
		return _spawnArray[selection];
	}
	else
	{
		return nullptr;
	}
}