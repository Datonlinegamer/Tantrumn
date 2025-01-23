// Fill out your copyright notice in the Description page of Project Settings.


#include "ATantrumnEndLevelTrigger.h"
#include "TantrumnGameModeBase.h"
#include "TanTrumnCharacterBase.h"
AATantrumnEndLevelTrigger::AATantrumnEndLevelTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &AATantrumnEndLevelTrigger::OnOverLapBegin);
}

void AATantrumnEndLevelTrigger::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = GetWorld()->GetAuthGameMode<ATantrumnGameModeBase>();
}

void AATantrumnEndLevelTrigger::OnOverLapBegin(AActor* OverLapActor, AActor* OtherActer)
{
	if (OtherActer == Cast<ATanTrumnCharacterBase>(OtherActer))
	{
		GameModeRef->PlayerReachedEnd();
	}
}
