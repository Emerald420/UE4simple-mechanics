// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	if (_HUDOverlayAsset)
	{
		_HUDOverlay = CreateWidget<UUserWidget>(this, _HUDOverlayAsset);
	}
	_HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	_HUDOverlay->AddToViewport();

	if (_wEnemyHealthBar)
	{
		_enemyHealthBar = CreateWidget<UUserWidget>(this, _wEnemyHealthBar);
		if (_enemyHealthBar)
		{
			_enemyHealthBar->AddToViewport();
			_enemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D alignment(0.f, 0.f);
		_enemyHealthBar->SetAlignmentInViewport(alignment);
	}
	
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (_enemyHealthBar)
	{
		_bEnemyHealthBarVisible = true;
		_enemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}

}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (_enemyHealthBar)
	{
		_bEnemyHealthBarVisible = true;
		_enemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_enemyHealthBar)
	{
		FVector2D positionViewPort;
		ProjectWorldLocationToScreen(_targetLocation,positionViewPort);
		positionViewPort.Y -= 50.f;
		FVector2D sizeInViewPort(200.f,15.f);

		_enemyHealthBar->SetPositionInViewport(positionViewPort);
		_enemyHealthBar->SetDesiredSizeInViewport(sizeInViewPort);
		
	}

}