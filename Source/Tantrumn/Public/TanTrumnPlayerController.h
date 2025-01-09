
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TanTrumnPlayerController.generated.h"
/**
 * 
 */

class UInputAction;
class UInputMappingContext;
class ATanTrumnCharacterBase;
struct  FInputActionValue;
UCLASS()
class TANTRUMN_API ATanTrumnPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATanTrumnPlayerController();

protected:
	void Move(const FInputActionValue& Value);
	void CameraLook(const FInputActionValue& Value);
	void PlayerJump(const FInputActionValue& Value);
	void SetupInputComponent();
	
	
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly)
	ATanTrumnCharacterBase* PC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction*CameraLookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputAction*JumpAction;
};
