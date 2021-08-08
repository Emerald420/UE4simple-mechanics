// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GamePlayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon()
{
	_skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	_skeletalMesh->SetupAttachment(GetRootComponent());

	_combatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	_combatCollision->SetupAttachment(GetRootComponent());
	
	_combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_combatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	_combatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	_combatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	_bActivateParticleSystem = false;
	_EweaponState = EWeaponState::EWS_Pickup;


	_damage = 25.f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	_combatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	_combatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex,bFromSweep,SweepResult);
	if ((_EweaponState == EWeaponState::EWS_Pickup)&& OtherActor)
	{
		AMyCharacter* character = Cast<AMyCharacter>(OtherActor);

		if (character)
		{
			character->SetOverlappingItem(this);
		}
	}
}


void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		AMyCharacter* character = Cast<AMyCharacter>(OtherActor);

		if (character)
		{
			character->SetOverlappingItem(nullptr);
		}
	}
}


void AWeapon::Equip(AMyCharacter* character)
{
	if (character)
	{
		_skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
		_skeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		_skeletalMesh->SetSimulatePhysics(false);

		SetInstigator(character->GetController());

		const USkeletalMeshSocket* lefthandSocket = character->GetMesh()->GetSocketByName("RightHandSocket");
		if (lefthandSocket)
		{
			lefthandSocket->AttachActor(this, character->GetMesh());
			_bRotate = false;
		
			character->SetEquippedWeapon(this);
			character->SetOverlappingItem(nullptr);
		
		}
		if (_onEquipSound)
		{
			UGameplayStatics::PlaySound2D(this, _onEquipSound);
		}
		if (!_bActivateParticleSystem)
		{
			_idleParticlesComponent->Deactivate();
		}
	}
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		if (enemy)
		{
			if (enemy->_hitParticle)
			{
				const USkeletalMeshSocket* WeaponSocket = _skeletalMesh->GetSocketByName("WeaponSocket");
				if (WeaponSocket)
				{
					FVector socketLocation = WeaponSocket->GetSocketLocation(_skeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), enemy->_hitParticle, socketLocation, FRotator(0.f), false);
				}
			}
			if (enemy->_hitSound)
			{
				UGameplayStatics::PlaySound2D(this,enemy->_hitSound);
			}
			if (_damageTypeClass)
			{
				UGameplayStatics::ApplyDamage(enemy, _damage, _weapongInstigator, this, _damageTypeClass);
			}
		}
	}
}


void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::ActivateCollision()
{
	_combatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UGameplayStatics::PlaySound2D(this,_swingSound);
}


void AWeapon::DeactivateCollision()
{
	_combatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
