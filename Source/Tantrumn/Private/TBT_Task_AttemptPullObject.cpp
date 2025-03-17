// Fill out your copyright notice in the Description page of Project Settings.


#include "TBT_Task_AttemptPullObject.h"
#include "TantrumnAIController.h"
#include "TanTrumnCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include"BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UTBT_Task_AttemptPullObject::UTBT_Task_AttemptPullObject(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NodeName = "Attempt Pull Object";
	bNotifyTick = false;
	bNotifyTaskFinished = true;

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UTBT_Task_AttemptPullObject, BlackboardKey));

}


EBTNodeResult::Type UTBT_Task_AttemptPullObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	const ATantrumnAIController* TantrumnAIController = Cast<ATantrumnAIController>(OwnerComp.GetOwner());
	ATanTrumnCharacterBase* TanTrumnCharacterBase = TantrumnAIController ? TantrumnAIController->GetPawn<ATanTrumnCharacterBase>() : nullptr;
	if (TanTrumnCharacterBase)
	{
		if (const UBlackboardComponent * MyBlackBoard = OwnerComp.GetBlackboardComponent())
		{
			const FVector TargetLocation = MyBlackBoard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
			if (TanTrumnCharacterBase->AttemptPullObjectAtLocation(TargetLocation))
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
