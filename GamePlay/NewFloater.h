// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define VECTORLOG(X,Y,Z) UE_LOG(LogTemp,Warning, TEXT("initialForce: x =%f, y= %f, z=%f"),X,Y,Z)

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewFloater.generated.h"


UCLASS()
class MYPROJECT3_API ANewFloater : public AActor
{
	GENERATED_BODY()


public:

	FVector _randomVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActorMeshComponent")
	UStaticMeshComponent* _staticMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector _initialLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector _placeLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Floater Variables")
	FVector _worldOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	FVector _InitialDirection;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector _initialForce;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector _initalTorque;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	bool _bShouldFloat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	bool _bInitializerFloaterLocation;

	


public:
	// Sets default values for this actor's properties
	ANewFloater();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
