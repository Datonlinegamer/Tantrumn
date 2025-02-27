// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractInterface.h"
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


USTRUCT(BlueprintType)
struct FEffectStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectType EffectType; // Fixed enum reference

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EffectStrength = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString EffectDescription = "";
};

UCLASS()

class TANTRUMN_API ATanTrumnCharacterBase : public ACharacter,public IInteractInterface
{
	GENERATED_BODY()

public:
	void RequestSprintStart();
	void RequestSprintEnd();
	void UnbindMontage();
	bool PlayThrowMontage();
	ATanTrumnCharacterBase();
	bool bIsStunned;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	void BaseCharacterCrouch();
	void BaseCharacterUnCrouch();
	void OnThrowableAttached(AThrowableActor* InThrowableActor);
	virtual void Landed(const FHitResult& Hit)override;
	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	void ResetThrowableObject();
	bool CanThrowObject() const { return CharacterThrowState == ECharacterThrowState::Attached; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	class UDataTable* EffectTable;

	void RequestThrowObject();
	void RequestPullObjectStart();
	void ProcessTraceResult(const FHitResult& HitResult);
	void RequestPullObjectStop();
	void RequestUsetObject();
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION(Server,Reliable)
	void ServerRequestThrowObject();



	

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRequestThrowObject();
	UFUNCTION(BlueprintPure)
	bool IsPullingObject() const { return CharacterThrowState == ECharacterThrowState::RequestingPull || CharacterThrowState == ECharacterThrowState::Pulling; }
	
	UFUNCTION(Server, Reliable)
	void ServerPullObject(AThrowableActor* InThrowableActor);

	UFUNCTION(Server, Reliable)
	void ServerRequestPullObject(bool bIsPulling);

	

	UFUNCTION(Client, Reliable)
	void ClientThrowableAttached(AThrowableActor* InThrowableActor);

	UFUNCTION(Server, Reliable)
	void ServerBeginThrow();

	UFUNCTION(Server, Reliable)
	void ServerFinishThrow();
	//UFUNCTION()
	//void OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION()
	void OnNotifyEndRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	void UpdateStun();

	void OnStunEnd();
	void OnRep_CharacterThrowState(const ECharacterThrowState& OldCharacterThrowState);
	float StunTime = 0.0f;
	float StunBeginTimestamp = 0.0f;

	void SphereCastPlayerView();

	void SphereCastActorTransform();

	void LineCastActorTransform();

	UPROPERTY(EditAnywhere, Category = "Throw", meta = (ClampMin = "0.0", Unit = "ms"))
	float ThrowSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	EEffectType SelectedEffectBP = EEffectType::None; // Fixed enum reference

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	EEffectType EffectTypes = EEffectType::None; // Fixed enum reference

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName SelectedEffect = " ";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float EffectStrength = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString Description = " ";
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
													// not working
	//UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_CharacterThrowState, Category = "Throw")

	ECharacterThrowState CharacterThrowState = ECharacterThrowState::None;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	EEffectType CurrentEffect = EEffectType::None;


	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyEffect_Implementation(EEffectType EffectType, bool bIsBuff) override;

	void EndEffect();
	bool bIsUnderEffect =false;
	bool bIsEffectBuff = false;
	float DefaultEffectCoolDown = 5.0f;
	float EffectCoolDown = 0.0f;
	float SprintSpeed = 900.0f;
};
