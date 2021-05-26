// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CustomAIController.generated.h"

/**
 * 
 */
class UBlackboardComponent;
class AAICharacter;
UCLASS()
class MOBILEACTIONRPG_API ACustomAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="BehaviorTree")
	UBehaviorTree* behaviorTree;
	UBlackboardComponent* blackboardComponent;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	AAICharacter* ownerCharacter;
};
