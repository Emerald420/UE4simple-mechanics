// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"


UCLASS()
class MYPROJECT3_API ACollidingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public: // user-declared

	// speed and velocity
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Speed")
	float _maxSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* _meshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USphereComponent* _sphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UCameraComponent* _camera;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USpringArmComponent* _springArm;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UMyPawnMovementComponent* _movementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;


	FTimerHandle timeManager;
	
	// Set-Get Mesh and Sphere
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return _meshComponent; }
	FORCEINLINE USphereComponent* GetSphereComponent() { return _sphereComponent; }
	FORCEINLINE void SetMeshComponent (UStaticMeshComponent* mesh) { _meshComponent = mesh; }
	FORCEINLINE void SetSphereComponent(USphereComponent* sphere) { _sphereComponent = sphere; }

	FORCEINLINE UCameraComponent* GetCameraComponent() { return _camera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return _springArm; }
	FORCEINLINE void SetCameraComponent(UCameraComponent* camera) { _camera = camera; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* springArm) { _springArm = springArm; }



private:
	// Move Function
	void MoveForward(float value);
	void MoveRight(float value);

	// Look Function
	void CameraYaw(float value);
	void CameraPitch(float value);

	FVector2D _cameraInput;
};
