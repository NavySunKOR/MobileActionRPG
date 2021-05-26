// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileActionRPG/AI/CustomAIController.h"
#include "MobileActionRPG/AI/AICharacter.h"
#include <BehaviorTree/BlackboardComponent.h>

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
	RunBehaviorTree(behaviorTree);
	blackboardComponent = GetBlackboardComponent();
}

void ACustomAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsBool("IsDead", ownerCharacter->isDead);
		blackboardComponent->SetValueAsVector("PlayerPosition",ownerCharacter->trackingPlayer->GetActorLocation());
		blackboardComponent->SetValueAsBool("IsInRange", ownerCharacter->isInRange);
	}

}