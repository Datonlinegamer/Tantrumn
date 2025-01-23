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
	DisplayCountDown();
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &ATantrumnGameModeBase::StartGame, GameCountDownDuration, false);
	CurrentGameState = EGameState::Waiting;
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

void ATantrumnGameModeBase::DisplayCountDown()
{
	if (!GameWidgetClass) { return; }
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GameWidget = CreateWidget<UTantrumnGameWidget>(PC, GameWidgetClass);
	GameWidget->AddToViewport();
	GameWidget->StartCountDown(GameCountDownDuration, this);
}

void ATantrumnGameModeBase::StartGame()
{
	CurrentGameState = EGameState::Playing;
}
