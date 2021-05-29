// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobileActionRPGGameModeBase.h"
#include "MobileActionRPG/AI/AICharacter.h"
#include "BattleFieldGameMode.generated.h"

/**
 * 
 */

class APlayerCharacter;

UCLASS()
class MOBILEACTIONRPG_API ABattleFieldGameMode : public AMobileActionRPGGameModeBase
{
	GENERATED_BODY()

private:

	bool isSpawned = false;
	TArray<AAICharacter*> enemyPool;

	UPROPERTY(EditAnywhere)
	float spawnCheckFrameRate;

	float spawnCheckTimer = 0.f;
	float spawnCheckInterval = 0.f;

	UPROPERTY(EditAnywhere)
	int spawnEnmeyAICount;

	UPROPERTY(EditAnywhere)
	FVector spawnTriggerPositionCenter;

	UPROPERTY(EditAnywhere)
	float spawnTriggerRadius;

	UPROPERTY(EditAnywhere)
	float spawnAreaMaximumRadius;

	UPROPERTY(EditAnywhere)
	float spawnAreaMinimumRadius;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAICharacter> enemyClass;

	APlayerCharacter* player;

	bool IsInSpawnTrigger();
	void SpawnEnemy();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	ABattleFieldGameMode();
	void AIKilled();
};
