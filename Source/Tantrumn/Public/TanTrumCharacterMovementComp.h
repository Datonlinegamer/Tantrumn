// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TanTrumCharacterMovementComp.generated.h"

/**
 * 
 */
class ATanTrumnCharacterBase;
UCLASS()
class TANTRUMN_API UTanTrumCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
    float OriginalCapsuleHalfHeight;
    UPROPERTY(EditDefaultsOnly)
    ATanTrumnCharacterBase* Player;
    // Function to handle movement mode change
    virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
    void ShrinkCapsule();
    void RestoreCapsule();
private:
   
	
};
