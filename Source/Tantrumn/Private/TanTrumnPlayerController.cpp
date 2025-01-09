// Fill out your copyright notice in the Description page of Project Settings.


#include "TanTrumnPlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TanTrumnCharacterBase.h"




ATanTrumnPlayerController::ATanTrumnPlayerController()
{
}


void ATanTrumnPlayerController::BeginPlay()
{
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(SubSystem);
	SubSystem->AddMappingContext(InputMapping, 0);
	PC = Cast<ATanTrumnCharacterBase>(GetPawn());
}



void ATanTrumnPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D input = Value.Get<FVector2D>();
	if (this == nullptr)
	{
		return;
	}
	const FRotator Rotate = GetControlRotation();
	const FRotator YawRotate(0, Rotate.Yaw, 0);
	const FVector ForwardDir = FRotationMatrix(YawRotate).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotate).GetUnitAxis(EAxis::Y);


	PC->AddMovementInput(ForwardDir, input.Y);
	PC->AddMovementInput(RightDir, input.X);


}

void ATanTrumnPlayerController::CameraLook(const FInputActionValue& Value)
{
	const FVector2D InputAxis = Value.Get<FVector2D>();
	PC->AddControllerYawInput(InputAxis.X);
	PC->AddControllerPitchInput(InputAxis.Y);
}

void ATanTrumnPlayerController::PlayerJump(const FInputActionValue& Value)
{
	const bool  InputAxis = Value.Get<bool>();
	PC->GetController()->GetCharacter()->Jump();
}

void ATanTrumnPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent);

	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATanTrumnPlayerController::Move);
	Input->BindAction(CameraLookAction, ETriggerEvent::Triggered, this, &ATanTrumnPlayerController::CameraLook);
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATanTrumnPlayerController::PlayerJump);
}