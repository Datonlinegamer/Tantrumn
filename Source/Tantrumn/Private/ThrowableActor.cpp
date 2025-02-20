// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TanTrumnCharacterBase.h"
#include "InteractInterface.h"
// Sets default values
AThrowableActor::AThrowableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>("StaticMesh");
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void AThrowableActor::BeginPlay()
{
	Super::BeginPlay();

	ToggleHighlight(false);
	State = Estate::Idle;
	Projectile->OnProjectileStop.AddDynamic(this, &AThrowableActor::ProjectileStop);
	//StaticMeshComponent->SetEnableGravity(true);
	///StaticMeshComponent->SetSimulatePhysics(true);

}

void AThrowableActor::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Projectile->OnProjectileStop.RemoveDynamic(this, &AThrowableActor::ProjectileStop);
	Super::EndPlay(EndPlayReason);
}

void AThrowableActor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if (State == Estate::Idle || State == Estate::Attached || State == Estate::Dropped)
	{
		return;
	}

	if (State ==Estate::Launch)
	{
		IInteractInterface* I = Cast<IInteractInterface>(Other);
		if (I)
		{
			I->Execute_ApplyEffect(Other, EffectType, false);

		}
	}
	if (PullActor && State == Estate::Pull)
	{
		if (ATanTrumnCharacterBase* TantrumnCharacter = Cast<ATanTrumnCharacterBase>(PullActor))
		{
			if (Other == PullActor)
			{
				AttachToComponent(TantrumnCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Objectsocket"));
				SetOwner(TantrumnCharacter);
				Projectile->Deactivate();
				State = Estate::Attached;
				TantrumnCharacter->OnThrowableAttached(this);
			}
			else
			{
				TantrumnCharacter->ResetThrowableObject();
				State = Estate::Dropped;
			}
		}
	}

	Projectile->HomingTargetComponent = nullptr;
	PullActor = nullptr;
}


void AThrowableActor::ProjectileStop(const FHitResult& ImpactResult)
{
	if (State == Estate::Launch || State == Estate::Dropped)
	{
		State = Estate::Idle;
	}
}

bool AThrowableActor::SetHomingTarget(AActor* Target)
{
	if (Target)
	{
		if (USceneComponent* SceneComponent = Cast<USceneComponent>(Target->GetComponentByClass(USceneComponent::StaticClass())))
		{
			if (USceneComponent* ThrowableSceneComponent = Cast <USceneComponent>(GetComponentByClass(USceneComponent::StaticClass())))
			{
				Projectile->SetUpdatedComponent(ThrowableSceneComponent);
				Projectile->Activate(true);
				Projectile->HomingTargetComponent = TWeakObjectPtr<USceneComponent>(SceneComponent);
				Projectile->Velocity = FVector(0.0f, 0.0f, 1000.0f);
				Projectile->bIsHomingProjectile = true;
				return true;
			}
		}
	}

	return false;
}

bool AThrowableActor::Pull(AActor* InActor)
{
	if (State != Estate::Idle)
	{
		return false;
	}

	if (SetHomingTarget(InActor))
	{
		ToggleHighlight(false);
		State = Estate::Pull;
		PullActor = InActor;
		return true;
	}

	return false;
}

void AThrowableActor::Launch(const FVector& InitialVelocity, AActor* Target)
{
	if (State == Estate::Pull || State == Estate::Attached)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		Projectile->Activate(true);
		Projectile->HomingTargetComponent = nullptr;

		State = Estate::Launch;

		if (Target)
		{
			if (USceneComponent* SceneComponent = Cast<USceneComponent>(Target->GetComponentByClass(USceneComponent::StaticClass())))
			{
				Projectile->HomingTargetComponent = TWeakObjectPtr<USceneComponent>(SceneComponent);
				return;
			}
		}

		Projectile->Velocity = InitialVelocity;
	}
}

void AThrowableActor::Drop()
{
	if (State == Estate::Attached)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	Projectile->Activate(true);
	Projectile->HomingTargetComponent = nullptr;
	State = Estate::Dropped;
}

EEffectType AThrowableActor::GetEffectType()
{
	return EffectType;
}

void AThrowableActor::ToggleHighlight(bool bIsOn)
{
	StaticMeshComponent->SetRenderCustomDepth(bIsOn);
}
// Called every frame
void AThrowableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

