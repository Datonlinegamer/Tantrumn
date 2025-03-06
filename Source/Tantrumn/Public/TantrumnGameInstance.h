// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <TantrumnGameWidget.cpp>
#include <TanTrumnPlayerController.h>
#include "TantrumnGameInstance.generated.h"

/**
 * 
 */
class ATantrumnGameStateBase;
UCLASS()
class TANTRUMN_API UTantrumnGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
		void DisplayCountdown(float GameCountdownDuration, ATanTrumnPlayerController* TantrumnPlayerController);
		void DisplayLevelComplete(ATanTrumnPlayerController* TantrumnPlayerController);
	
		UFUNCTION(BlueprintPure)
		ATantrumnGameStateBase* GetGameState() const { return TantrumnGameStateBase; }
	
		UFUNCTION(BlueprintCallable)
		void OnRetrySelected(ATanTrumnPlayerController* TantrumnPlayerController);
	
		UFUNCTION()
		void RestartGame(ATanTrumnPlayerController* TantrumnPlayerController);
	protected:
		UFUNCTION()
		void OnGameStateSet(AGameStateBase* GameStateBase);
	
	private:
		UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UTantrumnGameWidget> GameWidgetClass; // Exposed class to check the type of widget to display
	
		UPROPERTY()
		TMap<APlayerController*, UTantrumnGameWidget*> GameWidgets;
	
		UPROPERTY()
		ATantrumnGameStateBase* TantrumnGameStateBase;
	
};	