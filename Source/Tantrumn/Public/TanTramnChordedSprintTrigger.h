// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "TanTramnChordedSprintTrigger.generated.h"

/**
 * 
 */
class UInputAction;
UCLASS()
class TANTRUMN_API UTanTramnChordedSprintTrigger : public UInputTrigger
{
	GENERATED_BODY()
public:
	ETriggerState UpdateState(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime);

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SprintAction;
};
