#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TantrumnGameStateBase.generated.h"

UENUM(BlueprintType)
enum class ETantrumnGameState : uint8
{
	None		UMETA(DisplayName = "None"),
	Waiting		UMETA(DisplayName = "Waiting"),
	Playing		UMETA(DisplayName = "Playing"),
	Paused		UMETA(DisplayName = "Paused"),
	GameOver	UMETA(DisplayName = "GameOver"),
};

class ATanTrumnCharacterBase;

USTRUCT()
struct FGameResult
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	float Time;
};

UCLASS()
class TANTRUMN_API ATantrumnGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SetGameState(ETantrumnGameState InGameState) { GameState = InGameState; }

	UFUNCTION(BlueprintPure)
	ETantrumnGameState GetGameState() const { return GameState; }

	UFUNCTION(BlueprintPure)
	bool IsPlaying() const { return GameState == ETantrumnGameState::Playing; }

	//this will only be called on authority
	void OnPlayerReachedEnd(ATanTrumnCharacterBase* TantrumnCharacter);

	UFUNCTION()
	void ClearResults();

	

protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_GameState, Category = "States")
	ETantrumnGameState GameState = ETantrumnGameState::None;

	UFUNCTION()
	void OnRep_GameState(const ETantrumnGameState& OldGameState);

	UPROPERTY(VisibleAnywhere, replicated, Category = "States")
	TArray<FGameResult> Results;
};