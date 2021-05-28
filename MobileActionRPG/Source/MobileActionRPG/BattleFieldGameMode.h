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
	TArray<AAICharacter> enemyPool;

	bool isSpawned = false;

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

	//스폰 트리거 포지션에서 오프셋을 더함.
	UPROPERTY(EditAnywhere)
	FVector spawnAreaPositionOffset;

	UPROPERTY(EditAnywhere)
	float spawnAreaRadius;

	APlayerCharacter* player;

	bool IsInSpawnTrigger();
	void SpawnEnemy();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	ABattleFieldGameMode();
};
