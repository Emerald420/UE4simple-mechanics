// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class MYPROJECT3_API AMyPawn : public APawn
{
	GENERATED_BODY()

private:
	void MoveForward(float input);

	void MoveRight(float input);

public:
	// Sets default values for this pawn's properties
	AMyPawn();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:


	UPROPERTY(EditAnywhere,Category = "Mesh")
	USkeletalMeshComponent* _meshComponent;
	
	UPROPERTY(EditAnywhere,Category="Speed")
	float _maxSpeed;

	FVector _currentVelocity;

	class UCameraComponent* _camera;
};
