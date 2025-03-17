// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TBT_Task_AttemptPullObject.generated.h"

/**
 * 
 */
UCLASS()
class TANTRUMN_API UTBT_Task_AttemptPullObject : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UTBT_Task_AttemptPullObject(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
