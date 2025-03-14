// Fill out your copyright notice in the Description page of Project Settings.


#include "UTBT_TaskRequestStartSprint.h"
#include "TanTrumnCharacterBase.h"
#include "TantrumnAIController.h"

EBTNodeResult::Type UUTBT_TaskRequestStartSprint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);
	const	ATantrumnAIController* TantrumnAIController = Cast <ATantrumnAIController>(OwnerComp.GetOwner());
	ATanTrumnCharacterBase* TantrumnCharacterBase = TantrumnAIController ? TantrumnAIController->GetPawn < ATanTrumnCharacterBase>() : nullptr;
	if (TantrumnCharacterBase)
	{
		TantrumnCharacterBase->RequestSprintStart();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
