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

UCLASS()
class TANTRUMN_API ATantrumnGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	ATantrumnGameModeBase();

	virtual void BeginPlay() override;

	EGameState GetCurrentGameState();

	void PlayerReachedEnd();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States", meta = (AllowPrivateAccess = "true"))
	EGameState CurrentGameState = EGameState::None;

	UPROPERTY(EditAnywhere ,Category = "Game Details")
	float GameCountDownDuration;

	FTimerHandle TimeHandle;

	void DisplayCountDown();
	void StartGame();

};
