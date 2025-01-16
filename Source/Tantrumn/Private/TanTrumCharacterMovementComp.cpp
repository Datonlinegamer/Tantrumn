// Fill out your copyright notice in the Description page of Project Settings.

#include "TanTrumCharacterMovementComp.h"
#include "TanTrumnCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/GameplayStatics.h>
void UTanTrumCharacterMovementComp::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
}
void UTanTrumCharacterMovementComp::ShrinkCapsule()
{
    Player = Cast<ATanTrumnCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player)
    {
        UCapsuleComponent* Capsule = Player->GetCapsuleComponent();
      
            
            
            Capsule->SetCapsuleHalfHeight(OriginalCapsuleHalfHeight * 0.8f); // Shrink by 20%
       
    }
}

void UTanTrumCharacterMovementComp::RestoreCapsule()
{
    if (Player)
    {
        UCapsuleComponent* Capsule = Player->GetCapsuleComponent();
        if (Capsule && OriginalCapsuleHalfHeight > 0.f)
        {
            Capsule->SetCapsuleHalfHeight(OriginalCapsuleHalfHeight);
            OriginalCapsuleHalfHeight = 0.f; // Reset to prevent redundant operations
        }
    }
}
