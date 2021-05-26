// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileActionRPG/AI/BTDecorator_IsWaitTimeAvailable.h"
#include "MobileActionRPG/AI/CustomAIController.h"
#include "MobileActionRPG/AI/AICharacter.h"
#include <BehaviorTree/BlackboardComponent.h>

bool UBTDecorator_IsWaitTimeAvailable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACustomAIController* controller = Cast<ACustomAIController>(OwnerComp.GetAIOwner());

	if (controller && controller->ownerCharacter)
	{
		return controller->ownerCharacter->isAttack;
	}
	else
	{
		return false;
	}
}