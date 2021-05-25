// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileActionRPG/AI/AICharacter.h"
#include "MobileActionRPG/AI/CustomAIController.h"

void ACustomAIController::BeginPlay()
{
	Super::BeginPlay();
	ownerCharacter = Cast<AAICharacter>(GetPawn());
	if (ownerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setted"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No"));
	}
}