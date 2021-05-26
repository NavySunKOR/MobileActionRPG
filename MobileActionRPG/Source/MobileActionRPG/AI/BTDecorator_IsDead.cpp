// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileActionRPG/AI/BTDecorator_IsDead.h"
#include <BehaviorTree/BlackboardComponent.h>


bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	if (blackboard)
	{
		return blackboard->GetValueAsBool(TEXT("IsDead"));
	}
	else
	{
		return false;
	}
}