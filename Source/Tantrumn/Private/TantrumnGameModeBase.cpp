// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumnGameModeBase.h"
#include "TanTrumnCharacterBase.h"
#include "TantrumnGameWidget.h"
#include <Kismet/GameplayStatics.h>
ATantrumnGameModeBase::ATantrumnGameModeBase()
	:GameCountDownDuration(4.0f)
	,PC(nullptr)
{
	DefaultPawnClass = ATanTrumnCharacterBase::StaticClass();

}
void ATantrumnGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	DisplayCountdown();
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &ATantrumnGameModeBase::StartGame, GameCountDownDuration, false);
	CurrentGameState = EGameState::Waiting;
}

void ATantrumnGameModeBase::ReceivePlayer(APlayerController* PlayerController)
{
	AttemptStartGame();
}


void ATantrumnGameModeBase::AttemptStartGame()
{
	if (GetNumPlayers() == NumExpectedPlayers)
	{
		DisplayCountdown();
		if (GameCountDownDuration > SMALL_NUMBER)
		{
			GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &ATantrumnGameModeBase::StartGame, GameCountDownDuration, false);
		}
		else
		{
			StartGame();
		}

	}
}

EGameState ATantrumnGameModeBase::GetCurrentGameState()const
{
	return CurrentGameState;
}

void ATantrumnGameModeBase::PlayerReachedEnd()
{
	CurrentGameState = EGameState::GameOver;
	GameWidget->LevelComplete();
	FInputModeUIOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);
}

void ATantrumnGameModeBase::DisplayCountdown()
{
	if (!GameWidgetClass)
	{
		return;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController && PlayerController->PlayerState && !MustSpectate(PlayerController))
		{
			if (UTantrumnGameWidget* Widget = CreateWidget<UTantrumnGameWidget>(PlayerController, GameWidgetClass))
			{
				//GameWidget->AddToViewport();
				Widget->AddToPlayerScreen();
				Widget->StartCountDown(GameCountDownDuration, this);
				GameWidgets.Add(PlayerController, Widget);
			}
		}
	}
}

void ATantrumnGameModeBase::StartGame()
{
	CurrentGameState = EGameState::Playing;
	FInputModeGameOnly InputMode;
	if (!PC)return;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(false);
}
