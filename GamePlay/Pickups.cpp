// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"

APickups::APickups()
{

}

void APickups::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent,OtherActor, OtherComp,OtherBodyIndex, bFromSweep,SweepResult);

	if (OtherActor)
	{
		AMyCharacter* myCharacter= Cast<AMyCharacter>(OtherActor);
		
		if (myCharacter)
		{
			OnPickUpBP(myCharacter);

			if (_overlapParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _overlapParticles, GetActorLocation());
			}
			if (_overlapSound)
			{
				UGameplayStatics::PlaySound2D(this, _overlapSound);
			}
			myCharacter->pickupLocations.Add(GetActorLocation());
			myCharacter->ShowPickUp();
			Destroy();
		}
	}
}

void APickups::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent,OtherActor, OtherComp,OtherBodyIndex);

}