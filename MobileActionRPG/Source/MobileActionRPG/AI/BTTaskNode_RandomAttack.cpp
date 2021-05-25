// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileActionRPG/AI/BTTaskNode_RandomAttack.h"
#include "MobileActionRPG/AI/CustomAIController.h"

EBTNodeResult::Type UBTTaskNode_RandomAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    ACustomAIController* aiChar = Cast<ACustomAIController>(OwnerComp.GetAIOwner());
    if(aiChar == nullptr)
        return EBTNodeResult::Failed;

    if (aiChar && aiChar->ownerCharacter)
    {
        aiChar->ownerCharacter;
    }

    return EBTNodeResult::Succeeded;
}
