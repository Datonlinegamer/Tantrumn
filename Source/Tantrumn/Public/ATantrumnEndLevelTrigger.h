// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "ATantrumnEndLevelTrigger.generated.h"

/**
 * 
 */
class ATantrumnGameModeBase;
UCLASS()
class TANTRUMN_API AATantrumnEndLevelTrigger : public ATriggerVolume
{
	GENERATED_BODY()
public:

	AATantrumnEndLevelTrigger();

protected:
	virtual void BeginPlay()override;
private:
	UFUNCTION()
	void OnOverLapBegin(class AActor* OverLapActor, class AActor* OtherActer);

	ATantrumnGameModeBase* GameModeRef;
};
