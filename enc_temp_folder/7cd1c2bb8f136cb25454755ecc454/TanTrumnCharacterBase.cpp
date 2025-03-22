// Fill out your copyright notice in the Description page of Project Settings.


#include "TanTrumnCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TanTrumnPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "ThrowableActor.h"
#include "DrawDebugHelpers.h"
#include <Kismet/KismetSystemLibrary.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "TanTrumnCharacterBase.h"


constexpr int CVSphereCastPlayerView = 0;
constexpr int CVSphereCastActorTransform = 1;
constexpr int CVLineCastActorTransform = 2;

// Add cvars for debug
static TAutoConsoleVariable<int> CVarTraceMode(
	TEXT("Tantrum.Character.Debug.TraceMode"),
	0,
	TEXT("	0: Sphere cast PlayerView is used for direction/rotation (default).\n")
	TEXT("	1: Sphere cast using ActorTransform \n")
	TEXT("	2: Line cast using ActorTransform \n"),
	ECVF_Default);

static TAutoConsoleVariable<bool> CVarDisplayTrace(
	TEXT("Tantrum.Character.Debug.DisplayTrace"),
	false,
	TEXT("Display Trace"),
	ECVF_Default);

static TAutoConsoleVariable<bool> CVarDisplayThrowVelocity(
	TEXT("Tantrum.Character.Debug.DisplayThrowVelocity"),
	false,
	TEXT("Display Throw Velocity"),
	ECVF_Default);

ATanTrumnCharacterBase::ATanTrumnCharacterBase()
	
	:MinImpactSpeed(800.0f)
	,MaxImpactSpeed(1600.0f)
	,ThrowSpeed(2000.0f)
	,bIsStunned(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	cameraSpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(cameraSpringArm);
	bReplicates = true;
}


void ATanTrumnCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	EffectCoolDown = DefaultEffectCoolDown;
	cameraSpringArm->bEnableCameraLag = true;
	cameraSpringArm->bEnableCameraRotationLag = true;
	cameraSpringArm->CameraRotationLagSpeed = 10;
	cameraSpringArm->CameraLagSpeed = 10;
	PController = GetController<ATanTrumnPlayerController>();
	/*APlayerCameraManager* CameraManager = PController->PlayerCameraManager;
	if (CameraManager)
	{
		CameraManager->ViewPitchMax = 0.0f;
		CameraManager->ViewPitchMin = -32.0f;
	}*/
}

bool ATanTrumnCharacterBase::PlayCelebrateMontage()
{	
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = PlayAnimMontage(CelebrateMontage, PlayRate) > 0.f;
	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (!MontageEndedDelegate.IsBound())
		{
			MontageEndedDelegate.BindUObject(this, &ATanTrumnCharacterBase::OnMontageEnded);
		}
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, CelebrateMontage);
	}

	return bPlayedSuccessfully;
}
void ATanTrumnCharacterBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.Condition = COND_SkipOwner;
	//not working
	//DOREPLIFETIME_WITH_PARAMS_FAST(ATanTrumnCharacterBase, CharacterThrowState, SharedParams);


}
void ATanTrumnCharacterBase::BaseCharacterCrouch()
{
	Crouch();
}

void ATanTrumnCharacterBase::BaseCharacterUnCrouch()
{
	UnCrouch();
	
}
void ATanTrumnCharacterBase::OnThrowableAttached(AThrowableActor* InThrowableActor)
{
	CharacterThrowState = ECharacterThrowState::Attached;
	ThrowableActor = InThrowableActor;
	MoveIgnoreActorAdd(ThrowableActor);
}

void ATanTrumnCharacterBase::ResetThrowableObject()
{
	// Drop object
	if (ThrowableActor)
	{
		ThrowableActor->Drop();
	}
	CharacterThrowState = ECharacterThrowState::None;
	ThrowableActor = nullptr;
}
void ATanTrumnCharacterBase::RequestStopAiming()
{
	if (CharacterThrowState == ECharacterThrowState::Aiming)
	{
		CharacterThrowState = ECharacterThrowState::Attached;
		ServerRequestToggleAim(false);
	}
}
void ATanTrumnCharacterBase::ServerRequestToggleAim_Implementation(bool IsAiming)
{
	CharacterThrowState = IsAiming ? ECharacterThrowState::Aiming : ECharacterThrowState::Aiming;
}
void ATanTrumnCharacterBase::RequestThrowObject()
{
	if (CharacterThrowState != ECharacterThrowState::Attached)
	{
		return;
	}

	if (!CanThrowObject())
	{
		return;
	}

	if (!IsAiming())
	{
		CharacterThrowState = ECharacterThrowState::Aiming;
	}

	if (CanThrowObject() && CanAim())  // Ensure we can still throw and aim
	{
		CharacterThrowState = ECharacterThrowState::Throwing;
		ServerRequestThrowObject();
	}
	else
	{
		ResetThrowableObject();
	}
}

void ATanTrumnCharacterBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsLocallyControlled())
	{
		UnbindMontage();

	}

	if (Montage == ThrowMontage)
	{
		if (IsLocallyControlled())
		{
			CharacterThrowState = ECharacterThrowState::None;
			ServerFinishThrow();
			ThrowableActor;
		}
	}
	else
	{
		if (Montage == CelebrateMontage)
		{
			if (IsLocallyControlled())
			{
				/*if ()
				{

				}*/
			}
		}
	}
	CharacterThrowState = ECharacterThrowState::None;
	MoveIgnoreActorRemove(ThrowableActor);



	if (ThrowableActor)
	{

		if (ThrowableActor->GetRootComponent())
		{
			UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(ThrowableActor->GetRootComponent());
			if (RootPrimitiveComponent)
			{
				RootPrimitiveComponent->IgnoreActorWhenMoving(this, false);
			}
		}
		ThrowableActor = nullptr;
	}
}

void ATanTrumnCharacterBase::RequestSprintStart()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		GetCharacterMovement()->BrakingDecelerationWalking = 300.0f;
		GetCharacterMovement()->GroundFriction = 5.0f;
	}
}

void ATanTrumnCharacterBase::RequestSprintEnd()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 100.0f;
		GetCharacterMovement()->GroundFriction = 2.0f;
	}
}


void ATanTrumnCharacterBase::UnbindMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ATanTrumnCharacterBase::OnNotifyBeginReceived);
		AnimInstance->OnPlayMontageNotifyEnd.RemoveDynamic(this, &ATanTrumnCharacterBase::OnNotifyEndRecieved);
	}
}
bool ATanTrumnCharacterBase::PlayThrowMontage()
{
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = PlayAnimMontage(ThrowMontage, PlayRate) > 0.f;
	if (bPlayedSuccessfully)
	{
		if (IsLocallyControlled())
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (!BlendingOutDelegate.IsBound())
			{
				BlendingOutDelegate.BindUObject(this, &ATanTrumnCharacterBase::OnMontageBlendingOut);
			}
			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, ThrowMontage);

			if (!MontageEndedDelegate.IsBound())
			{
				MontageEndedDelegate.BindUObject(this, &ATanTrumnCharacterBase::OnMontageEnded);
			}
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, ThrowMontage);

			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ATanTrumnCharacterBase::OnNotifyBeginReceived);
			AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &ATanTrumnCharacterBase::OnNotifyEndRecieved);
		}
	}

	return bPlayedSuccessfully;
}
													

void ATanTrumnCharacterBase::RequestPullObjectStart()
{
	if (!bIsStunned && CharacterThrowState == ECharacterThrowState::None)
	{
		CharacterThrowState = ECharacterThrowState::RequestingPull;
		ServerRequestPullObject(true);
	}
}
void ATanTrumnCharacterBase::ProcessTraceResult(const FHitResult& HitResult,bool bHighLighting)
{
	
	AThrowableActor* HitThrowableActor = HitResult.bBlockingHit ? Cast<AThrowableActor>(HitResult.GetActor()) : nullptr;
	const bool IsSameActor = (ThrowableActor == HitThrowableActor);
	const bool IsValidTarget = HitThrowableActor && HitThrowableActor->IsIdle();

	// Clean up old actor
	if (ThrowableActor)
	{
		if (!IsValidTarget || !IsSameActor)
		{
			ThrowableActor->ToggleHighlight(false);
			ThrowableActor = nullptr;
		}
	}

	if (IsValidTarget)
	{
		if (!ThrowableActor)
		{
			ThrowableActor = HitThrowableActor;
			ThrowableActor->ToggleHighlight(true);
		}
	}
	if (!IsSameActor)
	{
		ThrowableActor = HitThrowableActor;

		if (bHighLighting)
		{
			if (ThrowableActor)
			{
				ThrowableActor->ToggleHighlight(true);

			}
		}
	}
	if (CharacterThrowState == ECharacterThrowState::RequestingPull)
	{
		// Don't allow for pulling objects while running/jogging
		if (GetVelocity().SizeSquared() < 100.0f)
		{
			if (ThrowableActor && ThrowableActor->Pull(this))
			{
				CharacterThrowState = ECharacterThrowState::Pulling;
				ThrowableActor = nullptr;
			}
		}
	}
}

void ATanTrumnCharacterBase::RequestPullObjectStop()
{
	// If we were pulling an object, drop it
	if (CharacterThrowState == ECharacterThrowState::RequestingPull)
	{
		CharacterThrowState = ECharacterThrowState::None;
		//ResetThrowableObject();
	}
}
void ATanTrumnCharacterBase::RequestUsetObject()
{
	ApplyEffect_Implementation(ThrowableActor->GetEffectType(), true);
	ThrowableActor->Destroy();
	ResetThrowableObject();
}
void ATanTrumnCharacterBase::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
}
void ATanTrumnCharacterBase::ServerPlayCelebrateMontage_Implementation()
{
	MulticastPlayCelebrateMontage();
}
void ATanTrumnCharacterBase::MulticastPlayCelebrateMontage_Implementation()
{
	PlayCelebrateMontage();
}
void ATanTrumnCharacterBase::ServerRequestThrowObject_Implementation()
{
	MultiCastRequestThrowObject();
}
void ATanTrumnCharacterBase::MultiCastRequestThrowObject_Implementation()
{
	if (IsLocallyControlled())
	{
		return;
	}

	PlayThrowMontage();
	CharacterThrowState = ECharacterThrowState::Throwing;
}

void ATanTrumnCharacterBase::ServerFinishThrow_Implementation()
{
	
	CharacterThrowState = ECharacterThrowState::None;

	MoveIgnoreActorRemove(ThrowableActor);
	if (ThrowableActor->GetRootComponent())
	{
		UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(ThrowableActor->GetRootComponent());
		if (RootPrimitiveComponent)
		{
			RootPrimitiveComponent->IgnoreActorWhenMoving(this, false);
		}
	}
	ThrowableActor = nullptr;
}
void ATanTrumnCharacterBase::ServerRequestPullObject_Implementation(bool bIsPulling)
{
	CharacterThrowState = bIsPulling ? ECharacterThrowState::RequestingPull : ECharacterThrowState::None;
}

void ATanTrumnCharacterBase::ServerPullObject_Implementation(AThrowableActor* InThrowableActor)
{
	if (InThrowableActor && InThrowableActor->Pull(this))
	{
		CharacterThrowState = ECharacterThrowState::Pulling;
		ThrowableActor = InThrowableActor;
		ThrowableActor->ToggleHighlight(false);
	}
}

void ATanTrumnCharacterBase::ClientThrowableAttached_Implementation(AThrowableActor* InThrowableActor)
{
	CharacterThrowState = ECharacterThrowState::Attached;
	ThrowableActor = InThrowableActor;
	MoveIgnoreActorAdd(ThrowableActor);
}

void ATanTrumnCharacterBase::ServerBeginThrow_Implementation()
{
	if (ThrowableActor->GetRootComponent())
	{
		UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(ThrowableActor->GetRootComponent());
		if (RootPrimitiveComponent)
		{
			RootPrimitiveComponent->IgnoreActorWhenMoving(this, true);
		}
	}
	
	const FVector& Direction = GetActorForwardVector() * ThrowSpeed;
	ThrowableActor->Launch(Direction);

	if (CVarDisplayThrowVelocity->GetBool())
	{
		const FVector& Start = GetMesh()->GetSocketLocation(TEXT("ObjectAttach"));
		DrawDebugLine(GetWorld(), Start, Start + Direction, FColor::Red, false, 5.0f);
	}
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
void ATanTrumnCharacterBase::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	// Ignore collisions otherwise the throwable object hits the player capsule and doesn't travel in the desired direction
	if (ThrowableActor && ThrowableActor->GetRootComponent())
	{
		UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(ThrowableActor->GetRootComponent());
		if (RootPrimitiveComponent)
		{
			RootPrimitiveComponent->IgnoreActorWhenMoving(this, true);
		}
	}

	// Ensure we get the correct throw direction
	const FVector Direction = GetActorForwardVector() * ThrowSpeed;
	if (ThrowableActor)
	{
		ThrowableActor->Launch(Direction);
	}

	if (CVarDisplayThrowVelocity->GetBool() && GetMesh())
	{
		const FVector Start = GetMesh()->GetSocketLocation(TEXT("Objectsocket"));
		DrawDebugLine(GetWorld(), Start, Start + Direction, FColor::Red, false, 5.0f);
	}
}

void ATanTrumnCharacterBase::RequestAim()
{
	if (!bIsStunned && CharacterThrowState == ECharacterThrowState::Attached)
	{
		CharacterThrowState = ECharacterThrowState::Aiming;
		ServerRequestToggleAim(true);
	}
}


void ATanTrumnCharacterBase::OnNotifyEndRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{

}
void ATanTrumnCharacterBase::UpdateStun()
{
	if (bIsStunned)
	{
		bIsStunned = (FApp::GetCurrentTime() - StunBeginTimestamp) < StunTime;
		if (!bIsStunned)
		{
			OnStunEnd();
		}
	}
}

void  ATanTrumnCharacterBase::OnStunEnd()
{
	StunBeginTimestamp = 0.0f;
	StunTime = 0.0f;
}
	
void ATanTrumnCharacterBase::OnRep_CharacterThrowState(const ECharacterThrowState& OldCharacterThrowState)
{
	if (CharacterThrowState != OldCharacterThrowState)
	{
		UE_LOG(LogTemp, Warning, TEXT("OldThrowState: %s"), *UEnum::GetDisplayValueAsText(OldCharacterThrowState).ToString());
		UE_LOG(LogTemp, Warning, TEXT("CharacterThrowState: %s"), *UEnum::GetDisplayValueAsText(CharacterThrowState).ToString());
	}
}

void  ATanTrumnCharacterBase::SphereCastPlayerView()
{
	FVector Location;
	FRotator Rotation;
	GetController()->GetPlayerViewPoint(Location, Rotation);
	const FVector PlayerViewForward = Rotation.Vector();
	const float AdditionalDistance = (Location - GetActorLocation()).Size();
	FVector EndPos = Location + (PlayerViewForward * (1000.0f + AdditionalDistance));

	const FVector CharacterForward = GetActorForwardVector();
	const float DotResult = FVector::DotProduct(PlayerViewForward, CharacterForward);
	// Prevent picking up objects behind us, this is when the camera is looking directly at the character's front side
	if (DotResult < -0.23f)
	{
		if (ThrowableActor)
		{
			ThrowableActor->ToggleHighlight(false);
			ThrowableActor = nullptr;
		}
		return;
	}

	FHitResult HitResult;
	EDrawDebugTrace::Type DebugTrace = CVarDisplayTrace->GetBool() ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	//bool UKismetSystemLibrary::SphereTraceSingle(const UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Location, EndPos, 70.0f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, DebugTrace, HitResult, true);
	ProcessTraceResult(HitResult);

#if ENABLE_DRAW_DEBUG
	if (CVarDisplayTrace->GetBool())
	{
		static float FovDeg = 90.0f;
		DrawDebugCamera(GetWorld(), Location, Rotation, FovDeg);
		DrawDebugLine(GetWorld(), Location, EndPos, HitResult.bBlockingHit ? FColor::Red : FColor::White);
		DrawDebugPoint(GetWorld(), EndPos, 70.0f, HitResult.bBlockingHit ? FColor::Red : FColor::White);
	}
#endif

}

void  ATanTrumnCharacterBase::SphereCastActorTransform()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + (GetActorForwardVector() * 1000.0f);

	// Sphere trace
	EDrawDebugTrace::Type DebugTrace = CVarDisplayTrace->GetBool() ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPos, EndPos, 70.0f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, TArray<AActor*>(), DebugTrace, HitResult, true);
	ProcessTraceResult(HitResult);
}

void  ATanTrumnCharacterBase::LineCastActorTransform()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + (GetActorForwardVector() * 1000.0f);
	FHitResult HitResult;
	GetWorld() ? GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECollisionChannel::ECC_Visibility) : false;
#if ENABLE_DRAW_DEBUG
	if (CVarDisplayTrace->GetBool())
	{
		DrawDebugLine(GetWorld(), StartPos, EndPos, HitResult.bBlockingHit ? FColor::Red : FColor::White, false);
	}
#endif
	ProcessTraceResult(HitResult);
}
void ATanTrumnCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsLocallyControlled())
	{
		return;
	}
	UpdateStun();
	if (bIsStunned)
	{
		return;
	}
	if (bIsUnderEffect)
	{
		if (EffectCoolDown > 0)
		{
			EffectCoolDown -= DeltaTime;
		}
		else
		{
			EffectCoolDown = false;
			EffectCoolDown = DefaultEffectCoolDown;
			EndEffect();
		}
	}

	if (CharacterThrowState == ECharacterThrowState::Throwing)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (UAnimMontage* CurrentAnimMontage = AnimInstance->GetCurrentActiveMontage())
			{
				const float PlayRate = AnimInstance->GetCurveValue(TEXT("ThrowCurve"));
				AnimInstance->Montage_SetPlayRate(CurrentAnimMontage, PlayRate);
			}
		}
	}



	else if (CharacterThrowState == ECharacterThrowState::None || CharacterThrowState == ECharacterThrowState::RequestingPull)
	{
		switch (CVarTraceMode->GetInt())
		{
		case CVSphereCastPlayerView:
			SphereCastPlayerView();
			break;
		case CVSphereCastActorTransform:
			SphereCastActorTransform();
			break;
		case CVLineCastActorTransform:
			LineCastActorTransform();
			break;
		default:
			SphereCastPlayerView();
			break;
		}
	}
}

void ATanTrumnCharacterBase::ApplyEffect_Implementation(EEffectType EffectType, bool bIsBuff)
{
	if (bIsUnderEffect) return;
	bIsUnderEffect = true;
	bIsEffectBuff = bIsBuff;
	switch (CurrentEffect)
	{
	case EEffectType::None:
		break;
	case EEffectType::Speed:
		//bIsEffectBuff ? SprintSpeed *= 2 : GetCharacterMovement()->DisableMovement(); <- does not work
		if (bIsEffectBuff)
		{
			SprintSpeed *= 2;
		}
		else
		{
			GetCharacterMovement()->DisableMovement();
		}
		break;
	case EEffectType::Jump:

		if (EffectTable)
		{
			static const FString ContextString(TEXT("Effect Context"));
			FEffectStats* Stats = EffectTable->FindRow<FEffectStats>(FName(SelectedEffect), ContextString, true);

			if (Stats)
			{
				EffectTypes = Stats->EffectType;
				EffectStrength = Stats->EffectStrength;
				Description = Stats->EffectDescription;
			}
		}
		break;
	case EEffectType::Power:
		break;
	default:
		break;
	}
		
}

void ATanTrumnCharacterBase::EndEffect()
{
	bIsUnderEffect = false;
	switch (CurrentEffect)
	{
	case EEffectType::None:
		break;
	case EEffectType::Speed:
		bIsEffectBuff ? SprintSpeed /= 2, RequestSprintEnd() : GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		break;
	case EEffectType::Jump:
		break;
	case EEffectType::Power:
		break;
	default:
		break;
	}
}

bool ATanTrumnCharacterBase::AttemptPullObjectAtLocation(const FVector& InLocation)
{
	if (CharacterThrowState != ECharacterThrowState::None && CharacterThrowState != ECharacterThrowState::RequestingPull)
	{
		return false;
		
	}

	FVector StartPos = GetActorLocation();
	FVector EndPos = InLocation;
	FHitResult HitResult;
	GetWorld() ? GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECollisionChannel::ECC_Visibility) : false;
#if ENABLE_DRAW_DEBUG
	if (CVarDisplayTrace->GetBool())
	{
		DrawDebugLine(GetWorld(), StartPos, EndPos, HitResult.bBlockingHit ? FColor::Red : FColor::White, false);
	}
#endif
	CharacterThrowState = ECharacterThrowState::RequestingPull;
	ProcessTraceResult(HitResult,false);
	if (CharacterThrowState == ECharacterThrowState::Pulling)
	{
		return true;
	}
	CharacterThrowState = ECharacterThrowState::None;
	return false;

}
