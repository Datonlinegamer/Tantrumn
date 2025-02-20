	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "ThrowableActor.generated.h"
class  UProjectileMovementComponent;
class UInteractInterface;
UCLASS()
class TANTRUMN_API AThrowableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	UFUNCTION(BlueprintCallable)
	void ToggleHighlight(bool bIsOn);

	AThrowableActor();

	UFUNCTION(BlueprintCallable)
	void Launch(const FVector& InitialVelocity, AActor* Target = nullptr);

	UFUNCTION(BlueprintCallable)
	bool Pull(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void Drop();

	UFUNCTION(BlueprintCallable)
	bool IsIdle() const { return State == Estate::Idle; }

	EEffectType GetEffectType();
protected:
	enum class Estate
	{
		Idle,
		Pull,
		Attached,
		Launch,
		Dropped
	};
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void BeginPlay() override;

	
	UFUNCTION(BlueprintCallable)
	bool SetHomingTarget(AActor* Target);
	
private:




	UFUNCTION()
	void ProjectileStop(const FHitResult& ImpactResult);

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;
	

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* Projectile;

	Estate State;

	UPROPERTY(EditAnywhere)
	AActor* PullActor = nullptr;
	
	UPROPERTY(EditAnywhere)
	EEffectType EffectType = EEffectType::None;
public:	
	
	virtual void Tick(float DeltaTime) override;

};
