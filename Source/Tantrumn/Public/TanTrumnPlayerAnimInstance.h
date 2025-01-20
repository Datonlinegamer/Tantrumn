	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TanTrumnPlayerAnimInstance.generated.h"

/**
 * 
 */
class ATanTrumnCharacterBase;

UCLASS()
class TANTRUMN_API UTanTrumnPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	

	UTanTrumnPlayerAnimInstance();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	ATanTrumnCharacterBase* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	bool bAccelerating;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool bIsInAir;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool Interact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool isAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "float")
	float PlayerSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool Crouch;
	
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation();
private:
	

	void PlayerMoveVelocity();
	
};
