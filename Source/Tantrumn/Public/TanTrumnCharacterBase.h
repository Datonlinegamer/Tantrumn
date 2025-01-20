// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TanTrumnCharacterBase.generated.h"
class UCameraComponent;
class USpringArmComponent;
class AThrowableActor;


UENUM(BlueprintType)
enum class ECharacterThrowState : uint8
{
	None			UMETA(DisplayName = "None"),
	RequestingPull	UMETA(DisplayName = "RequestingPull"),
	Pulling			UMETA(DisplayName = "Pulling"),
	Attached		UMETA(DisplayName = "Attached"),
	Throwing		UMETA(DisplayName = "Throwing"),
};

UCLASS()
class TANTRUMN_API ATanTrumnCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	void UnbindMontage();
	bool PlayThrowMontage();
	ATanTrumnCharacterBase();
	bool bIsStunned;
	void BaseCharacterCrouch();
	void BaseCharacterUnCrouch();
	void OnThrowableAttached(AThrowableActor* InThrowableActor);
	virtual void Landed(const FHitResult& Hit)override;
	void ResetThrowableObject();
	bool CanThrowObject() const { return CharacterThrowState == ECharacterThrowState::Attached; }

	void RequestThrowObject();
	void RequestPullObjectStart();
	void ProcessTraceResult(const FHitResult& HitResult);
	void RequestPullObjectStop();

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintPure)
	bool IsPullingObject() const { return CharacterThrowState == ECharacterThrowState::RequestingPull || CharacterThrowState == ECharacterThrowState::Pulling; }

	UFUNCTION()
	void OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION()
	void OnNotifyEndRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	void UpdateStun();

	void OnStunEnd();
	float StunTime = 0.0f;
	float StunBeginTimestamp = 0.0f;

	void SphereCastPlayerView();

	void SphereCastActorTransform();

	void LineCastActorTransform();

	UPROPERTY(EditAnywhere, Category = "Throw", meta = (ClampMin = "0.0", Unit = "ms"))
	float ThrowSpeed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private: 

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ThrowMontage = nullptr;

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

	UPROPERTY()
	AThrowableActor* ThrowableActor;

	UPROPERTY(VisibleAnywhere, Category = "Throw")
	ECharacterThrowState CharacterThrowState = ECharacterThrowState::None;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;



	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
