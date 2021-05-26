// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsWaitTimeAvailable.generated.h"

/**
 * 
 */
UCLASS()
class MOBILEACTIONRPG_API UBTDecorator_IsWaitTimeAvailable : public UBTDecorator
{
	GENERATED_BODY()


protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
};
