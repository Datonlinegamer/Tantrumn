// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumnGameStateBase.h"

#include "Net/UnrealNetwork.h"
#include "TantrumnCharacterBase.h"
#include "TantrumnPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TantrumnPlayerState.h"

void ATantrumnGameStateBase::OnPlayerReachedEnd(ATanTrumnCharacterBase* TantrumnCharacter)
{
	ensureMsgf(HasAuthority(), TEXT("ATantrumnGameStateBase::OnPlayerReachedEnd being called from Non Authority!"));
	if (ATanTrumnPlayerController* TantrumnPlayerController = TantrumnCharacter->GetController<ATanTrumnPlayerController>())
	{

		TantrumnPlayerController->ClientReachedEnd_Implementation();
		TantrumnCharacter->GetCharacterMovement()->DisableMovement();

		ATantrumnPlayerState* PlayerState = TantrumnPlayerController->GetPlayerState<ATantrumnPlayerState>();
		if (PlayerState)
		{
			const bool IsWinner = Results.Num() == 0;
			PlayerState->SetIsWinner(IsWinner);
			PlayerState->SetCurrentState(EPlayerGameState::Finished);
		}

		FGameResult Result;
		Result.Name = TantrumnCharacter->GetName();
		
		Result.Time = 5.0f;
		Results.Add(Result);

		
		if (Results.Num() == PlayerArray.Num())
		{
			GameState = ETantrumnGameState::GameOver;
		}
	}
}

void ATantrumnGameStateBase::ClearResults()
{
	Results.Empty();
}

void ATantrumnGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.Condition = COND_SkipOwner;

	DOREPLIFETIME_WITH_PARAMS_FAST(ATantrumnGameStateBase, GameState, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ATantrumnGameStateBase, Results, SharedParams);

	//DOREPLIFETIME(ATantrumnCharacterBase, CharacterThrowState);
}

void ATantrumnGameStateBase::OnRep_GameState(const ETantrumnGameState& OldGameState)
{
	UE_LOG(LogTemp, Warning, TEXT("OldGameState: %s"), *UEnum::GetDisplayValueAsText(OldGameState).ToString());
	UE_LOG(LogTemp, Warning, TEXT("GameState: %s"), *UEnum::GetDisplayValueAsText(GameState).ToString());
}