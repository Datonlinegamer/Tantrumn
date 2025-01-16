// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TanTrumnCharacterBase.generated.h"
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class TANTRUMN_API ATanTrumnCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATanTrumnCharacterBase();
	bool bCrouch;
	void BaseCharacterCrouch();
	void BaseCharacterUnCrouch();
	virtual void Landed(const FHitResult& Hit)override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private: 

	UPROPERTY(EditAnywhere, Category = "FallImpact")
	float MinImpactSpeed;

	UPROPERTY(EditAnywhere, Category = "FallImpact")
	float MaxImpactSpeed;

	bool bAffectLarge;
	bool bAffectSmall;

	APlayerController* PController;
	UPROPERTY(EditDefaultsOnly, Category = "CameraStuff")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "CameraStuff")
	USpringArmComponent* cameraSpringArm;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
