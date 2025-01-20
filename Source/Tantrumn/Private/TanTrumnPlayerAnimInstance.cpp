// Fill out your copyright notice in the Description page of Project Settings.


#include "TanTrumnPlayerAnimInstance.h"
#include "TanTrumnCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
UTanTrumnPlayerAnimInstance::UTanTrumnPlayerAnimInstance()
	:bIsInAir(false)
	,Crouch(false)
	,Interact(false)
{


}
void UTanTrumnPlayerAnimInstance::NativeInitializeAnimation()
{
	Player = Cast<ATanTrumnCharacterBase>(TryGetPawnOwner());
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
void UTanTrumnPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	PlayerMoveVelocity();	
}
