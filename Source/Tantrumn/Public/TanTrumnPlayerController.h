
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TanTrumnPlayerController.generated.h"
/**
 * 
 */
class ATantrumnGameModeBase;
class UInputAction;
class UInputMappingContext;
class ATanTrumnCharacterBase;
struct  FInputActionValue;
class ATanTrumnCharacterBase;
class UTanTrumCharacterMovementComp;

UCLASS()
class TANTRUMN_API ATanTrumnPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATanTrumnPlayerController(const FObjectInitializer& obj);
	bool Sprint;
	bool bInputAxis;
	bool pull;
	void Sprinting();
	void ClientReachedEnd_Implementation();
protected:
	float GroundCheckDistance = 100.0f;
	void Move(const FInputActionValue& Value);
	void CameraLook(const FInputActionValue& Value);
	void PlayerJump(const FInputActionValue& Value);
	void PlayerStartSprinting(const FInputActionValue& Value);
	void PlayerThrowObject(const FInputActionValue& Value);
	

	
	void RequestPullObjectStart(const FInputActionValue& Value);
	void SetupInputComponent();
	void Walking();
	void StopJumping();
	void ToggleCrouch();
	virtual void BeginPlay() override;

	void OnPossess(APawn* aPawn);

	virtual	void ReceivedPlayer() override;

private:
	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY()
	UUserWidget* HUDWidget;

	

	UPROPERTY(EditDefaultsOnly)
	ATanTrumnCharacterBase* PC;
	float JumpCooldownTime = 1.9f; // Cooldown duration in seconds
	float LastJumpTime = 0.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraLookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PullAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;


	ATantrumnGameModeBase* GameModeRef;
};


