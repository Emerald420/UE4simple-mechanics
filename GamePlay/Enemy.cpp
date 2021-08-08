// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "MyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_agroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	_agroSphere->SetupAttachment(GetRootComponent());
	_agroSphere->InitSphereRadius(600.f);


	_combatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	_combatSphere->SetupAttachment(GetRootComponent());
	_combatSphere->InitSphereRadius(130.f);

	_combatCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	_combatCollider->SetupAttachment(GetMesh(), FName("EnemySocket"));
	
	_bOverlappingCombatSphere = false;

	_MAXHP = 100.f;

	_HP = 75.f;

	_damage = 10.f;

	_attackMinTime = 0.5f;
	_attackMaxTime = 1.5f;
	
	_EEnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;



}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_AIController = Cast<AAIController>(GetController());

	_agroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	_agroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	_agroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Ignore);
	

	_combatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	_combatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
	_combatSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);


	_combatCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	_combatCollider->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);



	_combatCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_combatCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	_combatCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	_combatCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
		if (OtherActor)
		{
			_target = character;
			MoveToTarget(character);
		}
		
	}
}
void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor &&Alive())
	{
		AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
		if (character)
		{
			if (character)
			{
				character->SetHasCombatTarget(false);
				character->UpdateCombatTarget();
			}
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (_AIController)
			{

				_AIController->StopMovement();
				_target = nullptr;
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
		if (character)
		{
			_target = character;
			character->SetInterpTarget(this);
			character->SetHasCombatTarget(true);
			if (character->_mainPlayerController)
			{
				character->_mainPlayerController->DisplayEnemyHealthBar();
			}
	
			_bOverlappingCombatSphere = true;
			Attack();
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor&&Alive()&&OtherComp)
	{
		AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
		if (character)
		{
			_bOverlappingCombatSphere = false;
			if (character->_InterpTarget == this)
			{
				character->SetInterpTarget(nullptr);
				character->SetHasCombatTarget(false);
				character->UpdateCombatTarget();
			}
			if (_EEnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
			{
				MoveToTarget(character);
			}
			if (character->_mainPlayerController)
			{
				USkeletalMeshComponent* mainMesh = Cast<USkeletalMeshComponent>(OtherComp);
				if (mainMesh)
				{
					character->UpdateCombatTarget();
				}
			}
			GetWorldTimerManager().ClearTimer(_attackTimer);
		}
	}
}

void AEnemy::MoveToTarget(AMyCharacter* target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
	
	if (_AIController)
	{
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalActor(target);
		moveRequest.SetAcceptanceRadius(15.f);

		FNavPathSharedPtr navPath;
		_AIController->MoveTo(moveRequest, &navPath);
		
		if (navPath)
		{
			auto pathPoints = navPath->GetPathPoints();
			for (auto points : pathPoints)
			{
				FVector location = points.Location;
				UKismetSystemLibrary::DrawDebugSphere(this, location, 25.f, 8, FLinearColor::Green, 10.f, 0.5f);
			}
		}
	}
}


void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMyCharacter* character = Cast<AMyCharacter>(OtherActor);
		if (character)
		{
			if (character->_hitParticle)
			{
				const USkeletalMeshSocket* tipSocket = GetMesh()->GetSocketByName("TipSocket");
				if (tipSocket)
				{
					FVector socketLocation = tipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), character->_hitParticle, socketLocation, FRotator(0.f), false);
				}
			}
			if (character->_hitSound)
			{
				UGameplayStatics::PlaySound2D(this, character->_hitSound);
			}
			if (_damageTypeClass)
			{
				UGameplayStatics::ApplyDamage(character,_damage,_AIController,this,_damageTypeClass);
			}
		}
	}
}


void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::ActivateCollision()
{
	UE_LOG(LogTemp, Warning, TEXT("act!"));
	_combatCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (_SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, _SwingSound);
	}
}

void AEnemy::DeactivateCollision()
{
	UE_LOG(LogTemp, Warning, TEXT("deact!"));
	_combatCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (Alive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));	
		if (_AIController&&!_bAttacking && _target)
		{
			_target->UpdateCombatTarget();
			_AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
			if (_target->_EMovementStatus == EMovementStatus::EMS_Dead)
			{
				return;
			}
			FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _target->GetActorLocation());
			SetActorRotation(lookAtRotation);
			_bAttacking = true;
			UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
			if (animInstance)
			{
				animInstance->Montage_Play(_enemyCombatMontage, 1.35f);
				animInstance->Montage_JumpToSection(FName("Attack"), _enemyCombatMontage);
			}
		}
	}
}

void AEnemy::AttackEnd()
{
	_bAttacking = false;
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
	if (_bOverlappingCombatSphere)
	{
		float attackTime = FMath::FRandRange(_attackMinTime, _attackMaxTime);
		GetWorldTimerManager().SetTimer(_attackTimer, this, &AEnemy::Attack, attackTime);
	}
	else if(_target)
	{
		MoveToTarget(_target);
	}
}


float AEnemy::TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser)
{
	_HP -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("HP - %f"), _HP);
	if (_HP <= 0.f)
	{
		Die(DamageCauser);
	}
	return DamageAmount;
}


void AEnemy::Die(AActor* causer)
{
	if (_target)
	{
		_target->SetHasCombatTarget(false);
		_target->_mainPlayerController->RemoveEnemyHealthBar();
	}
	
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && _enemyCombatMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("It Died"), _HP);
		animInstance->Montage_Play(_enemyCombatMontage, 1.f);
		animInstance->Montage_JumpToSection(FName("Die"),_enemyCombatMontage);
	}
	GetWorldTimerManager().ClearTimer(_attackTimer);
	
	_bAttacking = false;

	_combatCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_agroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_combatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AMyCharacter* character = Cast<AMyCharacter>(causer);
	if (character)
	{
		character->UpdateCombatTarget();
	}

}

void AEnemy::DeathEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Notify Work"));
	GetMesh()->bPauseAnims = true;
	GetWorldTimerManager().SetTimer(_deathTimer, this, &AEnemy::Disappear,5.f);
	
}

bool AEnemy::Alive()
{
	return _EEnemyMovementStatus != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
	Destroy();
}
