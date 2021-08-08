// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_transitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = _transitionVolume;

	_billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	_billboard->SetupAttachment(GetRootComponent());

	_transitionLevelName = "Sun Temple";
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();
	_transitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);

}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
		if (character)
		{
			SwitchLevel(_transitionLevelName);
		}
	}
}

void ALevelTransitionVolume::SwitchLevel(FName levelName)
{
	UWorld* world = GetWorld();
	if (world)
	{
		FString currentLevel = world->GetMapName();
		FName currentLevelName = FName(currentLevel);
		if (currentLevelName !=levelName)
		{
			UGameplayStatics::OpenLevel(world, levelName);
		}
	}

}