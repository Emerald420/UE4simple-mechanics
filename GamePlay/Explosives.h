// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Explosives.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API AExplosives : public AItems
{
	GENERATED_BODY()
	
public:

	AExplosives();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="Damage")
	float _damage;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category ="Combat")
	TSubclassOf<UDamageType> _damageTypeClass;

};
