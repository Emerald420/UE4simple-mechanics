// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Pickups.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API APickups : public AItems
{
	GENERATED_BODY()
	
public:

	APickups();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickUpBP(class AMyCharacter* target);

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	
};
