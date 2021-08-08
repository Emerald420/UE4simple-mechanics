// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> _HUDOverlayAsset;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Widgets")
	UUserWidget* _HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> _wEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* _enemyHealthBar;

	bool _bEnemyHealthBarVisible; 

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector _targetLocation;

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;
};
