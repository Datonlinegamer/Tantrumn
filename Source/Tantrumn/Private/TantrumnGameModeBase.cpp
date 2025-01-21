// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumnGameModeBase.h"
#include "TanTrumnCharacterBase.h"
ATantrumnGameModeBase::ATantrumnGameModeBase()
	:GameCountDownDuration(4.0f)
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

EGameState ATantrumnGameModeBase::GetCurrentGameState()
{
	return CurrentGameState;
}

void ATantrumnGameModeBase::PlayerReachedEnd()
{
	CurrentGameState = EGameState::GameOver;
}

void ATantrumnGameModeBase::DisplayCountDown()
{

}

void ATantrumnGameModeBase::StartGame()
{
	CurrentGameState = EGameState::Playing;
}
