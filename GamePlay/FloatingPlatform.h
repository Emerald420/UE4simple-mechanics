// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class MYPROJECT3_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();


	// Mesh for the platform
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "StaticMesh")
	class UStaticMeshComponent* _staticMesh;

	UPROPERTY(EditAnywhere)
	FVector _startPoint;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector _endPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float _interpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float _interpTime;

	FTimerHandle _interpTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool _bInterp;


	float _distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();

	void SwapVectors(FVector& VecOne, FVector& VecTwo);
};
