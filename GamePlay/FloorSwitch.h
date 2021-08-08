// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "FloorSwitch.generated.h"

UCLASS()
class MYPROJECT3_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UBoxComponent* _triggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	class UStaticMeshComponent* _floorSwitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	UStaticMeshComponent* _door;

	UPROPERTY(BlueprintReadWrite,Category = "Floor Switch")
	FVector _initialDoorLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector _initialSwitchLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	float _switchTime;

	FTimerHandle _switchHandle;

	bool _bCharacterOnSwitch;


	/*
	FTimeline myTimeLine;
	FVector initialLoc;
	FVector targetLoc
	
	UFUNCTION()
	void HandleProgress(float value);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	float MaxBounceHeight;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;
	*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateDoorLocation(float z);

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateFloorSwitchLocation(float z);

	void CloseDoor();


};
