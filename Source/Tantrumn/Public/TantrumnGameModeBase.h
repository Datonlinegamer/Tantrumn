// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TantrumnGameModeBase.generated.h"
UENUM(BlueprintType)
enum class EGameState : uint8
{
	None UMETA(DisplayName = "None"),
	Waiting UMETA(DisplayName = "Waiting"),
	Playing UMETA(DisplayName = "Playing"),
	Paused UMETA(DisplayName = "Paused"),
	GameOver UMETA(DisplayName = "GameOver"),
};
/**
 * 
 */
class UTantrumnGameWidget;
UCLASS()
class TANTRUMN_API ATantrumnGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	ATantrumnGameModeBase();

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	EGameState GetCurrentGameState()const ;

	void PlayerReachedEnd();


private:
	UPROPERTY(VisibleAnywhere,Category = "States")
	EGameState CurrentGameState = EGameState::None;

	UPROPERTY(EditAnywhere ,Category = "Game Details")
	float GameCountDownDuration;

	FTimerHandle TimeHandle;

	UPROPERTY()
	UTantrumnGameWidget* GameWidget;

	UPROPERTY(EditDefaultsOnly, Category = "GameWidget")
	TSubclassOf<UTantrumnGameWidget>GameWidgetClass;

	APlayerController* PC;
	void DisplayCountDown();
	void StartGame();

};
