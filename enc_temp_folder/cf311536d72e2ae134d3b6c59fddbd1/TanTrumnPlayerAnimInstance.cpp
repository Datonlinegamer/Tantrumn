// Fill out your copyright notice in the Description page of Project Settings.


#include "TanTrumnPlayerAnimInstance.h"
#include "TanTrumnCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TantrumnGameModeBase.h"
UTanTrumnPlayerAnimInstance::UTanTrumnPlayerAnimInstance()
	:bIsInAir(false)
	,Crouch(false)
	,Interact(false)
{


}
void UTanTrumnPlayerAnimInstance::NativeInitializeAnimation()
{
	Player = Cast<ATanTrumnCharacterBase>(TryGetPawnOwner());

	if (UWorld* World = GetWorld())
	{
		GameModeRef = Cast<ATantrumnGameModeBase>(World->GetAuthGameMode());
	}
	else
	{
		GameModeRef = nullptr; // Explicitly set to nullptr for clarity
	}
}

void UTanTrumnPlayerAnimInstance::PlayerMoveVelocity()
{
	if (Player)
	{
		FVector Velocity = Player->GetVelocity();
		PlayerSpeed = Velocity.Size();
		bAccelerating = Player->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;
		bIsInAir = Player->GetCharacterMovement()->IsFalling();

		
		Crouch = Player->bIsCrouched;

		
	}
}
void UTanTrumnPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	PlayerMoveVelocity();	
	
	if (GameModeRef)
	{
		EGameState CurrentGameAnimationState = GameModeRef->GetCurrentGameState();

		if (CurrentGameAnimationState == EGameState::Waiting)
		{
			Hovering = true;
		}
		else if (CurrentGameAnimationState == EGameState::Playing)
		{
			Hovering = false;
		}
	}

	if (Player)
	{
		if (!Player->IsPullingObject())
		{
			Interact =false;
		}
		else
		{
			Interact = true;
		}
			
	}
}
