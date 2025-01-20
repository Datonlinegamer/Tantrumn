// Fill out your copyright notice in the Description page of Project Settings.

#include "TanTrumnPlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TanTrumnCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TanTrumCharacterMovementComp.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

ATanTrumnPlayerController::ATanTrumnPlayerController(const FObjectInitializer& obj)
    : Super(obj), Sprint(false)
{
    // Ensure the movement component is initialized
    moveComp = CreateDefaultSubobject<UTanTrumCharacterMovementComp>(TEXT("MoveComp"));
}

void ATanTrumnPlayerController::BeginPlay()
{
    Super::BeginPlay();

    PC = Cast<ATanTrumnCharacterBase>(GetPawn());
    if (PC)
    {
        Walking();
       

        // Disable controller rotation and enable movement orientation
        PC->bUseControllerRotationPitch = false;
        PC->bUseControllerRotationYaw = false;
        PC->bUseControllerRotationRoll = false;
        PC->GetCharacterMovement()->bOrientRotationToMovement = true;
        PC->GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    }

    // Add the input mapping context
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
        if (SubSystem && InputMapping)
        {
            SubSystem->AddMappingContext(InputMapping, 0);
        }
    }
}

void ATanTrumnPlayerController::Move(const FInputActionValue& Value)
{
    if (!PC) return;

    FVector2D Input = Value.Get<FVector2D>();
    const FRotator Rotate = GetControlRotation();
    const FRotator YawRotate(0, Rotate.Yaw, 0);
    const FVector ForwardDir = FRotationMatrix(YawRotate).GetUnitAxis(EAxis::X);
    const FVector RightDir = FRotationMatrix(YawRotate).GetUnitAxis(EAxis::Y);

    PC->AddMovementInput(ForwardDir, Input.Y);
    PC->AddMovementInput(RightDir, Input.X);
}

void ATanTrumnPlayerController::CameraLook(const FInputActionValue& Value)
{
    if (!PC) return;

    FVector2D InputAxis = Value.Get<FVector2D>();
    PC->AddControllerYawInput(InputAxis.X);
    PC->AddControllerPitchInput(InputAxis.Y);
}

void ATanTrumnPlayerController::PlayerJump(const FInputActionValue& Value)
{
    bInputAxis = Value.Get<bool>();
    if (!PC) // Ensure PC is valid
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerCharacter reference (PC) is null."));
        return;
    }

    UCharacterMovementComponent* MovementComp = PC->GetCharacterMovement();
    if (!MovementComp) return;

    const float CurrentTime = GetWorld()->GetTimeSeconds(); // Get the current world time
    if (CurrentTime - LastJumpTime < JumpCooldownTime) // Check if cooldown is active
    {
        UE_LOG(LogTemp, Log, TEXT("Jump is on cooldown."));
        return;
    }

    if (!MovementComp->IsFalling() && bInputAxis)
    {
        // Configure jump-related properties
        MovementComp->JumpZVelocity = 500.0f;
        MovementComp->GravityScale = 0.9f;
        MovementComp->AirControl = 0.2f;

        UE_LOG(LogTemp, Log, TEXT("Player jumped."));

        if (PC->bIsCrouched)
        {
            PC->StopJumping();
        }
        else
        {
            PC->Jump();
        }

        LastJumpTime = CurrentTime; // Update the last jump time
    }
}


void ATanTrumnPlayerController::PlayerStartSprinting(const FInputActionValue& Value)
{
    Sprint = Value.Get<bool>();
    if (PC)
    {
        if (Sprint)
        {
            Sprinting();
        }
        else
        {
            Walking();
        }
    }
}



void ATanTrumnPlayerController::Walking()
{
    if (PC && PC->GetCharacterMovement())
    {
        PC->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
        PC->GetCharacterMovement()->BrakingDecelerationWalking = 100.0f;
        PC->GetCharacterMovement()->GroundFriction = 2.0f;
    }
}

void ATanTrumnPlayerController::Sprinting()
{
    if (PC && PC->GetCharacterMovement())
    {
        PC->GetCharacterMovement()->MaxWalkSpeed = 900.0f;
        PC->GetCharacterMovement()->BrakingDecelerationWalking = 300.0f;
        PC->GetCharacterMovement()->GroundFriction = 5.0f;
    }
}

void ATanTrumnPlayerController::StopJumping()
{
    if (PC)
    {
        PC->StopJumping();
    }
}

void ATanTrumnPlayerController::ToggleCrouch()
{
    if (PC->bIsCrouched)
    {
        PC->BaseCharacterUnCrouch();
    }
    else
    {
        PC->BaseCharacterCrouch();
    }
}
void ATanTrumnPlayerController::RequestPullObjectStart(const FInputActionValue& Value)
{

    if (!PC->IsPullingObject())
    {
        PC->RequestPullObjectStart();
    }
    else
    {

        PC->RequestPullObjectStop();
    }
}

void ATanTrumnPlayerController::PlayerThrowObject(const FInputActionValue& Value)
{
    if (PC->CanThrowObject())
    {
        PC->PlayThrowMontage();
    }
}

void ATanTrumnPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent);
    if (Input)
    {
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATanTrumnPlayerController::Move);
        Input->BindAction(CameraLookAction, ETriggerEvent::Triggered, this, &ATanTrumnPlayerController::CameraLook);
        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATanTrumnPlayerController::PlayerJump);
        Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ATanTrumnPlayerController::PlayerStartSprinting);
        Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATanTrumnPlayerController::PlayerStartSprinting);
        Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &ATanTrumnPlayerController::ToggleCrouch);
        Input->BindAction(PullAction, ETriggerEvent::Started, this, &ATanTrumnPlayerController::RequestPullObjectStart);
        Input->BindAction(PullAction, ETriggerEvent::Completed, this, &ATanTrumnPlayerController::RequestPullObjectStart);
        Input->BindAction(ThrowAction, ETriggerEvent::Started, this, &ATanTrumnPlayerController::PlayerThrowObject);
        
    }
}
