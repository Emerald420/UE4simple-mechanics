// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	EWS_Pickup UMETA(DisplayName = "Pickup"),
	EWS_Equip UMETA(DisplayName = "Equipped"),
	EWS_MAX UMETA(DisplayName = "DefaultMax")

};


/**
 * 
 */
UCLASS()
class MYPROJECT3_API AWeapon : public AItems
{
	GENERATED_BODY()

public:
	
	AWeapon();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category ="Item")
	EWeaponState _EweaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* _onEquipSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* _skeletalMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Item | Combat")
	class UBoxComponent* _combatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	bool _bActivateParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	float _damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	class USoundCue* _swingSound;


protected:
	virtual void BeginPlay()override;


public:


	FORCEINLINE void SetWeaponState(EWeaponState state) { _EweaponState = state; }
	FORCEINLINE EWeaponState SetWeaponState() { return _EweaponState; }
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	void Equip(class AMyCharacter* character);


	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Combat")
	TSubclassOf<UDamageType> _damageTypeClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat")
	AController* _weapongInstigator;

	FORCEINLINE void SetInstigator(AController* inst) { _weapongInstigator = inst; }
};
