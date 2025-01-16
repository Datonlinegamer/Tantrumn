// Fill out your copyright notice in the Description page of Project Settings.


#include "TanTramnChordedSprintTrigger.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"

ETriggerState UTanTramnChordedSprintTrigger::UpdateState(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{

	if (!MoveAction||SprintAction)
	{
		return ETriggerState::None;
	}

	FVector2D MoveValue = PlayerInput->GetActionValue(MoveAction).Get<FVector2D>();

	bool Sprinting = PlayerInput->GetActionValue(SprintAction).Get<bool>();

	if (MoveValue.X > 0.0f&& Sprinting)
	{
		return ETriggerState::Triggered;
	}
	return ETriggerState();
}
