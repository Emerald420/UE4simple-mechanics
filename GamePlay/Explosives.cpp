// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosives.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"

AExplosives::AExplosives()
{
	_damage = 15.f;

}


void AExplosives::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor)
	{
		ACharacter* actor = Cast<ACharacter>(OtherActor);
		AMyCharacter* myCharacter = Cast<AMyCharacter>(OtherActor);
		AEnemy* enemy = Cast<AEnemy>(OtherActor);;
		
		if (myCharacter|| enemy)
		{
			if (_overlapParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _overlapParticles, GetActorLocation());
			}
			if (_overlapSound)
			{
				UGameplayStatics::PlaySound2D(this, _overlapSound);
			}

			UGameplayStatics::ApplyDamage(OtherActor, _damage, nullptr, this, _damageTypeClass);

			FVector location = actor->GetActorForwardVector();
			float mass = actor->GetCharacterMovement()->Mass;
			actor->GetCharacterMovement()->AddImpulse(-location* mass * 1000.f + FVector(0.f,0.f, mass*100));
		
			Destroy();
		}
	}

}

void AExplosives::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	

}

