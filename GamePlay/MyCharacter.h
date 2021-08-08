// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"


UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName ="Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhausedRecovering"),
	ESS_MAX UMETA(DisplayName = "DefaultMax")
};


UENUM()
enum class EWallRunningDirection : int8
{
	ERD_Left = 1 UMETA(DisplayName = "Left"),
	ERD_Right = -1 UMETA(DisplayName = "Right"),

	ERD_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM()
enum class EComboAttack :int8
{
	ECA_Normal UMETA(DisplayName = "Normal"),
	ECA_First UMETA(DisplayName = "FirstStrike"),
	ECA_Second UMETA(DisplayName = "SecondStrike"),

	ECA_MAX UMETA(DisplayName = "DefaultMAX")

};


UCLASS()
class MYPROJECT3_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	//For Debugging
	TArray<FVector> pickupLocations;
	UFUNCTION(BlueprintCallable)
	void ShowPickUp();
	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AWeapon* _equippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AItems* _overlappingItem;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category ="Enums")
	EMovementStatus _EMovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus _EStaminaStatus;

	EComboAttack _EComboAttackState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* _hitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* _hitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float _runningSpeed;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Running")
	float _sprintingSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* _springArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAceess = "true"))
	class UCameraComponent* _camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float _baseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float _baseLookUpRate;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Player Stats")
	float _maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	float _health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Player Stats")
	float _maxStatmina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	float _stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	int32 _coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float _gravityWhileRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float _wallRunningDirection;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Movement")
	float _staminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float _minSprintStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float _minZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float _maxZoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anims")
	bool _bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Combat")
	class AEnemy* _InterpTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool _bHasCombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* _mainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool _bInterpToEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector _combatTargetLocation;

	bool _bIsMoving;

	FTimerHandle _timer;

	float _interpSpeed;

	bool _bFromBackWall;

	bool _bShiftKeyDown;

	bool _bEKeyDown;

	bool _bisRunningWall;

	float _timeCounter;

	bool _bLMBDown;
	
	bool _bComboStart;


	//how to change gravity
	//GetCharacterMovement()->GravityScale = 0.1f; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser)override;

	virtual void Jump()override;

	void MoveForward(float value);

	void MoveRight(float value);

	void LookUp(float value);

	void LookRight(float value);
	
	void TurnAtRate(float rate);

	void LookUpAtRate(float rate);

	void Zoom(float value);

	void SetMovementStatus(EMovementStatus status);

	void DecrementHealth(float amount);


	UFUNCTION(BlueprintCallable)
	void AddCoins(int32 coins);

	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float amount);

	void Die();

	void RunWall();

	void StopRunWall();

	void EKeyDown();

	void EKeyUp();

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void ComboStart();

	UFUNCTION(BlueprintCallable)
	void ComboEnd();

	void LMBKeyDown();

	void LMBKeyUp();

	void SetComboState(EComboAttack state);

	void ClearComboState();

	void SetInterpToEnemy(bool Interp);

	void UpdateCombatTarget();

	FRotator GetLookAtRotationYaw(FVector target);

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return _springArm; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return _camera; }
	FORCEINLINE class UCharacterMovementComponent* GetCharacterMovementComponent() const { return _characterMovementComponent; }
	FORCEINLINE void SetStaminaStatus(EStaminaStatus status){_EStaminaStatus = status;}
	FORCEINLINE void ShiftKeyDown(){ _bShiftKeyDown = true; }
	FORCEINLINE void ShiftKeyUp() { _bShiftKeyDown = false; }	
	void SetEquippedWeapon(AWeapon* weapon);
	FORCEINLINE void SetOverlappingItem(AItems* item) { _overlappingItem =item; }
	FORCEINLINE AWeapon* GetEquippedWeapon() { return _equippedWeapon; }
	FORCEINLINE void SetInterpTarget(AEnemy* enemy) { _InterpTarget = enemy; }
	FORCEINLINE void SetHasCombatTarget(bool hasTarget) { _bHasCombatTarget = hasTarget; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* _combatMontage;

	class UCharacterMovementComponent* _characterMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemy> _enemyFilter;


private :

	APlayerController* _playerController;

	FVector _wallRunningVector;

	float _originalGravity;

	float _wallRunningSpeed;

};
