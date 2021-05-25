// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CustomAIController.generated.h"

/**
 * 
 */
class AAICharacter;
UCLASS()
class MOBILEACTIONRPG_API ACustomAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
public:
	AAICharacter* ownerCharacter;
};
