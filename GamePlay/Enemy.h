// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking UMETA(DisplayName = "Attacking"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class MYPROJECT3_API AEnemy : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus _EEnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus status) { _EEnemyMovementStatus = status; }

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AI")
	class USphereComponent* _agroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* _combatSphere;

	class AAIController* _AIController;

	FTimerHandle _attackTimer;

	FTimerHandle _deathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float _attackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float _attackMaxTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool _bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category ="AI")
	AMyCharacter* _target;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI")
	float _MAXHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float _HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float _damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UParticleSystem* _hitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* _hitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* _SwingSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* _combatCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Combat")
	class UAnimMontage* _enemyCombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool _bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Combat")
	TSubclassOf<UDamageType> _damageTypeClass;

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION()
		virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser)override;

	UFUNCTION(BlueprintCallable)
		void MoveToTarget(class AMyCharacter* target);

	UFUNCTION(BlueprintCallable)
		void DeathEnd();


	bool Alive();


	void Die(AActor* causer);

	void Disappear();

};
