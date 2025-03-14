// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumnAIController.h"
#include	"TanTrumnCharacterBase.h"
#include "TantrumnPlayerState.h"
void ATantrumnAIController::OnPossess(APawn*IsPawn)
{
	Super::OnPossess(IsPawn);
	if (ATanTrumnCharacterBase* TantrumnCharacter = Cast<ATanTrumnCharacterBase>(IsPawn))
	{
		if (ATantrumnPlayerState* TantrumnPlayerState = GetPlayerState<ATantrumnPlayerState>())
		{
			TantrumnPlayerState->SetCurrentState(EPlayerGameState::Waiting);

		}
	}
}

void ATantrumnAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ATantrumnAIController::OnReachedEnd()
{
	if (ATanTrumnCharacterBase* TantrumnCharacterBase = Cast<ATanTrumnCharacterBase>(GetCharacter()))
	{
		TantrumnCharacterBase->ServerPlayCelebrateMontage();
	}
}
