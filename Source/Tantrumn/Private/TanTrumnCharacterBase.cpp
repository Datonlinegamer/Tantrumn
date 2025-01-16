// Fill out your copyright notice in the Description page of Project Settings.


#include "TanTrumnCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TanTrumnPlayerController.h"
#include "Components/CapsuleComponent.h"
// Sets default values
ATanTrumnCharacterBase::ATanTrumnCharacterBase()
	:bCrouch(false)
	,MinImpactSpeed(800.0f)
	,MaxImpactSpeed(1600.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	cameraSpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(cameraSpringArm);
}


void ATanTrumnCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	cameraSpringArm->bEnableCameraLag = true;
	cameraSpringArm->bEnableCameraRotationLag = true;
	cameraSpringArm->CameraRotationLagSpeed = 10;
	cameraSpringArm->CameraLagSpeed = 10;
	PController = GetController<ATanTrumnPlayerController>();
	APlayerCameraManager* CameraManager = PController->PlayerCameraManager;
	if (CameraManager)
	{
		CameraManager->ViewPitchMax = 0.0f;
		CameraManager->ViewPitchMin = -32.0f;
	}
}

void ATanTrumnCharacterBase::BaseCharacterCrouch()
{
	Crouch();
}

void ATanTrumnCharacterBase::BaseCharacterUnCrouch()
{
	UnCrouch();
	
}

void ATanTrumnCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	ATanTrumnPlayerController* PlayerController = GetController<ATanTrumnPlayerController>();
	if (PlayerController)
	{
		const float FallImpactSpeed =FMath::Abs(GetVelocity().Z);
		if (FallImpactSpeed < MinImpactSpeed)
		{
			return;
		}
		const float DeltaImpact = MaxImpactSpeed - MinImpactSpeed;
		const float FallRatio = FMath::Clamp((FallImpactSpeed - MinImpactSpeed) / DeltaImpact, 0.0f, 1.0f);
		bAffectSmall = FallRatio <= 0.5;
		bAffectLarge = FallRatio > 0.5;
		PlayerController->PlayDynamicForceFeedback(FallRatio, 0.5f, bAffectLarge, bAffectSmall, bAffectLarge, bAffectSmall);
	}
}


	
void ATanTrumnCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



