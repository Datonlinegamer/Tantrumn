// Fill out your copyright notice in the Description page of Project Settings.


#include "TanTrumnCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Sets default values
ATanTrumnCharacterBase::ATanTrumnCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	cameraSpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(cameraSpringArm);
}

// Called when the game starts or when spawned
void ATanTrumnCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATanTrumnCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input

