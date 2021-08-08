// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "MainPlayerController.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArm->SetupAttachment(GetRootComponent());
	_springArm->TargetArmLength = 400.f;
	_springArm->bUsePawnControlRotation = true;

	GetCapsuleComponent()->SetCapsuleSize(30.f,90.f);

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_camera->SetupAttachment(_springArm,USpringArmComponent::SocketName);
	_camera->bUsePawnControlRotation = false;

	_baseTurnRate = 65.f;
	_baseLookUpRate = 65.f;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	_characterMovementComponent = GetCharacterMovement();

	_characterMovementComponent->bUseControllerDesiredRotation = false;
	_characterMovementComponent->bOrientRotationToMovement = true;
	
	_characterMovementComponent->RotationRate = FRotator(0.f, 540.f,0.f);
	_characterMovementComponent->JumpZVelocity = 450.f;
	_characterMovementComponent->AirControl = 0.01f;


	AutoPossessPlayer = EAutoReceiveInput::Player0;


	_minZoom = 250.f;
	_maxZoom = 650.f;

	_maxHealth = 100.f;
	_health = 65.f;
	_maxStatmina = 150.f;
	_stamina = 120.f;
	_coins = 0;
	_bisRunningWall = false;
	_originalGravity = _characterMovementComponent->GravityScale;
	_gravityWhileRunning = 0.1f;

	_runningSpeed = 500.f;
	_sprintingSpeed = 650.f;
	_bShiftKeyDown = false;
	_bEKeyDown = false;

	_EMovementStatus = EMovementStatus::EMS_Normal;
	_EStaminaStatus = EStaminaStatus::ESS_Normal;

	_staminaDrainRate = 25.f;
	_minSprintStamina = 50.f;

	_interpSpeed = 15.f;
	_bInterpToEnemy = false;

	_bHasCombatTarget = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	_playerController = GetWorld()->GetFirstPlayerController();
	_mainPlayerController = Cast<AMainPlayerController>(GetController());
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_EMovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	float deltaStamina = _staminaDrainRate * DeltaTime;

	_bIsMoving = _characterMovementComponent->Velocity.Size() == 0 ? false : true;
	switch (_EStaminaStatus)
	{
		case EStaminaStatus::ESS_Normal:
		{
			if (_bShiftKeyDown && _bIsMoving)
			{
				if (_stamina - deltaStamina <= _minSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
					_stamina -= deltaStamina;
				}
				else
				{
					_stamina -= deltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else
			{
				if (_stamina + deltaStamina >= _maxStatmina)
				{
					_stamina = _maxStatmina;
				}
				else
				{
					_stamina += deltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			break;
		}
		case EStaminaStatus::ESS_BelowMinimum:
		{
			if (_bShiftKeyDown && _bIsMoving)
			{
				if (_stamina - deltaStamina <= 0.f)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
					_stamina = 0;
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
				else
				{
					_stamina -= deltaStamina;
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
			}
			else
			{
				if (_stamina + deltaStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Normal);
					_stamina += deltaStamina;
				}
				else
				{
					_stamina += deltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			break;
		}
		case EStaminaStatus::ESS_Exhausted:
		{
			if (_bShiftKeyDown && _bIsMoving)
			{
				_stamina = 0.f;
			}
			else
			{
				SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
				_stamina += deltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
			break;
		}
		case EStaminaStatus::ESS_ExhaustedRecovering:
		{
			if (_stamina + deltaStamina >= _minSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				_stamina += deltaStamina;
			}
			else
			{
				_stamina += deltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
			break;
		}
		default:
		{
			break;
		}
	}

	if (_bInterpToEnemy && _InterpTarget)
	{
		FRotator lookAtYaw = GetLookAtRotationYaw(_InterpTarget->GetActorLocation());
		FRotator interpolation = FMath::RInterpTo(GetActorRotation(), lookAtYaw,DeltaTime,_interpSpeed);
	
		SetActorRotation(interpolation);
	}

	if (_InterpTarget)
	{
		_combatTargetLocation = _InterpTarget->GetActorLocation();
		if (_mainPlayerController)
		{
			_mainPlayerController->_targetLocation = _combatTargetLocation;
		}
	}

	//Debug for wall run
	#pragma region Debug
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorForwardVector()* UINT_MAX, FColor::Red);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorRightVector()* UINT_MAX, FColor::Green);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorUpVector()* UINT_MAX, FColor::Blue);

	FHitResult wallHit;
	FCollisionObjectQueryParams collisionType;
	collisionType.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	GetWorld()->LineTraceSingleByObjectType(wallHit, GetActorLocation(), GetActorForwardVector()* UINT_MAX, collisionType);
	AActor* wall = Cast<AActor>(wallHit.Actor);
	if (wall)
	{
		FVector actorForwardVector = GetActorForwardVector();
		actorForwardVector.Normalize();
		FVector hitlocationVector = wallHit.Location;
		DrawDebugLine(GetWorld(), hitlocationVector, wallHit.ImpactNormal * UINT_MAX, FColor::Purple);
	}
	#pragma endregion Debug

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::ShiftKeyUp);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AMyCharacter::EKeyDown);
	PlayerInputComponent->BindAction("Equip", IE_Released, this, &AMyCharacter::EKeyUp);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::LMBKeyDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMyCharacter::LMBKeyUp);


	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis("Zoom", this, &AMyCharacter::Zoom);
	
}

void AMyCharacter::MoveForward(float value)
{

	if ((Controller != nullptr) && (value != 0.0f) &&!_bAttacking && (_EMovementStatus != EMovementStatus::EMS_Dead))
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.f, rotation.Yaw, 0.f);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value);
	}
}

void AMyCharacter::MoveRight(float value)
{
	
	if ((Controller != nullptr) && (value != 0.0f) && !_bAttacking && (_EMovementStatus != EMovementStatus::EMS_Dead))
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.f, rotation.Yaw, 0.f);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}

void AMyCharacter::LookUp(float value)
{
	FRotator baseAimRotator = GetBaseAimRotation();
	
	if (value >=0)
	{
		if (baseAimRotator.Pitch > -45.f)
		{
			AddControllerPitchInput(value);
		}
	}
	else
	{
		if (baseAimRotator.Pitch < 25.f)
		{
			AddControllerPitchInput(value);
		}
	}

}

void AMyCharacter::LookRight(float value)
{
}

void AMyCharacter::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * _baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * _baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::Zoom(float value)
{
	FVector newLoc = GetActorLocation() - _camera->GetComponentLocation();
	if ((newLoc.Size() < _minZoom && value>0) ||(newLoc.Size() > _maxZoom && value < 0))
		return;
	newLoc.Normalize();	
	_camera->AddWorldOffset(newLoc * value);
}

void AMyCharacter::DecrementHealth(float amount)
{
	_health -= amount;
	if (_health <= 0.f)
	{
		Die();
	}
}

void AMyCharacter::AddCoins(int32 coins)
{
	_coins += coins;
}

void AMyCharacter::Die()
{
	if (_EMovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && _combatMontage)
	{
		animInstance->Montage_Play(_combatMontage, 1.0f);
		animInstance->Montage_JumpToSection(FName("Death"),_combatMontage);
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
}



void AMyCharacter::Jump()
{
	if (_EMovementStatus != EMovementStatus::EMS_Dead)
	{
		if (_bisRunningWall)
		{
			StopRunWall();
			return;
		}

		FHitResult wallHit;
		FCollisionObjectQueryParams collisionType;
		collisionType.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
		GetWorld()->LineTraceSingleByObjectType(wallHit, GetActorLocation(), GetActorForwardVector() * UINT_MAX, collisionType);
		AActor* wall = Cast<AActor>(wallHit.Actor);

		if (!_bisRunningWall && wall == nullptr)
		{
			ACharacter::Jump();
			return;
		}

		FVector actorForwardVector = GetActorForwardVector();
		actorForwardVector.Normalize();

		FVector wallNormal = wallHit.ImpactNormal * UINT_MAX;
		wallNormal.Normalize();

		_wallRunningVector = wall->GetActorForwardVector();
		_wallRunningVector.Normalize();

		if (FVector::Dist(wallHit.Location, wall->GetActorLocation() + wall->GetActorRightVector()) > FVector::Dist(wallHit.Location, wall->GetActorLocation() - wall->GetActorRightVector()))
		{
			UE_LOG(LogTemp, Warning, TEXT("yeah"));
			_bFromBackWall = true;
		}

		if (!_bFromBackWall && FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(-actorForwardVector, _wallRunningVector))) > 90)
		{
			_wallRunningDirection = static_cast<int8>(EWallRunningDirection::ERD_Left);
		}
		else if (!_bFromBackWall && FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(-actorForwardVector, _wallRunningVector))) < 90)
		{
			_wallRunningDirection = static_cast<int8>(EWallRunningDirection::ERD_Right);
		}
		else if (_bFromBackWall && FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(-actorForwardVector, -_wallRunningVector))) < 90)
		{
			_wallRunningDirection = static_cast<int8>(EWallRunningDirection::ERD_Right);
			_wallRunningVector = -_wallRunningVector;
		}
		else
		{
			_wallRunningDirection = static_cast<int8>(EWallRunningDirection::ERD_Left);
			_wallRunningVector = -_wallRunningVector;
		}
		_bFromBackWall = false;

		UE_LOG(LogTemp, Warning, TEXT("hitlocation dot actorlocation %f"), (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(-actorForwardVector, wallNormal)))));


		if (FVector::Dist(wallHit.Location, GetActorLocation()) > 120.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("wallnotHit"));
			ACharacter::Jump();
			return;
		}
		else if ((!_bisRunningWall) && FVector::Dist(wallHit.Location, GetActorLocation()) < 120.f && (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(-actorForwardVector, wallNormal))) < 90))
		{
			UE_LOG(LogTemp, Warning, TEXT("wallRunnable %f degree"), FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(-actorForwardVector, wallNormal))));
			_timeCounter = 0;
			_characterMovementComponent->GravityScale = 0;
			_characterMovementComponent->Velocity = FVector(0.f, 0.f, 0.f);
			_wallRunningSpeed = _characterMovementComponent->MaxWalkSpeed;
			_bisRunningWall = true;
			_playerController->SetIgnoreMoveInput(true);
			GetWorldTimerManager().SetTimer(_timer, this, &AMyCharacter::RunWall, GetWorld()->GetDeltaSeconds(), true);

		}
	}
}


void AMyCharacter::RunWall()
{
	if (_timeCounter < 1.3f && _bisRunningWall)
	{
		_timeCounter += GetWorld()->GetDeltaSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f sec"), _timeCounter)
		FVector newlocation = GetActorLocation();
		newlocation += _wallRunningVector*_wallRunningDirection * (_wallRunningSpeed *(GetWorld()->GetDeltaSeconds()));
		if (_timeCounter < 0.3f)
		{
			newlocation += FVector(0,0, 70.f * (GetWorld()->GetDeltaSeconds()));
			SetActorLocation(newlocation);
		}
		else
		{
			newlocation += FVector(0, 0, -50.f * (GetWorld()->GetDeltaSeconds()));
			SetActorLocation(newlocation);
		}
		UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), newlocation.X, newlocation.Y, newlocation.Z);
	}
	if (_timeCounter>1.3f)
	{
		StopRunWall();
	}
	

}

void AMyCharacter::StopRunWall()
{
	GetWorldTimerManager().ClearTimer(_timer);
	_playerController->SetIgnoreMoveInput(false);
	FVector impulseSide = GetActorRightVector();
	if (_wallRunningDirection == static_cast<int8>(EWallRunningDirection::ERD_Right))
	{
		impulseSide = -impulseSide;
	}
	
	FVector impulseUp = GetActorUpVector();
	impulseSide.Normalize();
	impulseUp.Normalize();

	_characterMovementComponent->AddImpulse((impulseSide+impulseUp)*_characterMovementComponent->Mass*400.f);
	_characterMovementComponent->GravityScale = _originalGravity;
	SetActorRotation(GetActorRotation()+FRotator(0.f,60.f*_wallRunningDirection,0.f));
	_bisRunningWall = false;
}

void AMyCharacter::SetMovementStatus(EMovementStatus status)
{
	_EMovementStatus = status;
	if (_EMovementStatus == EMovementStatus::EMS_Sprinting)
	{
		_characterMovementComponent->MaxWalkSpeed = _sprintingSpeed;
	}
	else if(_EMovementStatus ==EMovementStatus::EMS_Normal)
	{
		_characterMovementComponent->MaxWalkSpeed = _runningSpeed;
	}
}


//Debug
void AMyCharacter::ShowPickUp()
{
	for (const auto& loc : pickupLocations)
	{
		DrawDebugSphere(GetWorld(), loc, 5.f, 12, FColor::Red, false, 10.f);
	}
}

void AMyCharacter::EKeyDown()
{
	_bEKeyDown = true;
	if (_overlappingItem)
	{
		AWeapon* weapon = Cast<AWeapon>(_overlappingItem);
		if (weapon)
		{
			weapon->Equip(this);
			SetOverlappingItem(nullptr);
		}
	}
}

void AMyCharacter::EKeyUp()
{
	_bEKeyDown = false;
}

void AMyCharacter::SetEquippedWeapon(AWeapon* weapon)
{
	if (_equippedWeapon)
	{
		_equippedWeapon->Destroy();
	}
	_equippedWeapon = weapon;
}

void AMyCharacter::Attack()
{
	if ((_bComboStart || _bAttacking == false) && (_EMovementStatus !=EMovementStatus::EMS_Dead))
	{
		SetInterpToEnemy(true);
		_bAttacking = true;
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && _combatMontage)
		{
			switch (_EComboAttackState)
			{
				case EComboAttack::ECA_Normal:
				{
					animInstance->Montage_Play(_combatMontage, 2.f);
					animInstance->Montage_JumpToSection(FName("AttackFirst"), _combatMontage);
					SetComboState(EComboAttack::ECA_First);
					GetWorldTimerManager().ClearTimer(_timer);
					GetWorldTimerManager().SetTimer(_timer, this,&AMyCharacter::ClearComboState,1,false, 1.5f);
					break;
				}	
				case EComboAttack::ECA_First:
				{
					animInstance->Montage_Play(_combatMontage, 2.5f);
					animInstance->Montage_JumpToSection(FName("AttackSecond"), _combatMontage);
					SetComboState(EComboAttack::ECA_Second);
					GetWorldTimerManager().ClearTimer(_timer);
					ClearComboState();
					break;
				}
				default:
					break;
			}
		}

	}
}

void AMyCharacter::LMBKeyDown()
{
	if (_EMovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}
	_bLMBDown = true;
	if (_equippedWeapon) 
	{
		Attack();
	}
	
}

void AMyCharacter::LMBKeyUp()
{
	_bLMBDown = false;
}

void AMyCharacter::AttackEnd()
{
	_bAttacking = false;
	SetInterpToEnemy(false);
}

void AMyCharacter::SetComboState(EComboAttack state)
{
	_EComboAttackState = state;
}

void AMyCharacter::ClearComboState()
{
	_EComboAttackState = EComboAttack::ECA_Normal;
	ComboEnd();
}

void AMyCharacter::ComboStart()
{
	_bComboStart = true;
}

void AMyCharacter::ComboEnd()
{
	_bComboStart = false;
}

void AMyCharacter::SetInterpToEnemy(bool Interp)
{
	_bInterpToEnemy = Interp;
}

FRotator AMyCharacter::GetLookAtRotationYaw(FVector target)
{
	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),target);
	return FRotator(0.f, lookAtRotation.Yaw, 0.f);
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	DecrementHealth(DamageAmount);
	return DamageAmount;
}

void AMyCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMyCharacter::UpdateCombatTarget()
{
	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors,_enemyFilter);
	if (overlappingActors.Num()==0)
	{
		if (_mainPlayerController)
		{
			_mainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}
	FVector location = GetActorLocation();
	float minDistanceToActor = FLT_MAX;
	AEnemy* closestEnemy = Cast<AEnemy>(overlappingActors[0]);
	for (auto actor : overlappingActors)
	{
		AEnemy* enemy = Cast<AEnemy>(actor);
		if (enemy)
		{
			if ((enemy->GetActorLocation() - location).Size() < minDistanceToActor)
			{
				minDistanceToActor = (enemy->GetActorLocation() - location).Size();
				closestEnemy = enemy;
			}
		}
	}
	if (_mainPlayerController)
	{
		_mainPlayerController->DisplayEnemyHealthBar();
	}
	if (closestEnemy)
	{
		SetHasCombatTarget(true);
		SetInterpTarget(closestEnemy);
	}
}

void AMyCharacter::IncrementHealth(float amount)
{
	if (_health + amount >= _maxHealth)
	{
		_health = _maxHealth;
	}
	else
	{
		_health += amount;
	}
}
