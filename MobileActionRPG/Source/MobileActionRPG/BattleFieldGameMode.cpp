// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleFieldGameMode.h"
#include "MobileActionRPG/Player/PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

ABattleFieldGameMode::ABattleFieldGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}
void ABattleFieldGameMode::AIKilled()
{
	int count = 0;
	for (int i = 0; i < enemyPool.Num(); i++)
	{
		if (enemyPool[i]->IsHidden())
		{
			count++;
		}
	}
	if (count == spawnEnmeyAICount)
	{
		isSpawned = false;
	}
}
bool ABattleFieldGameMode::IsInSpawnTrigger()
{
	FVector playerPosition = player->GetActorLocation();
	playerPosition.Z = 0.f;
	spawnTriggerPositionCenter.Z = 0.f;

	FVector triggerPos = playerPosition - spawnTriggerPositionCenter;

	float pow2 = triggerPos.X * triggerPos.X + triggerPos.Y * triggerPos.Y;

	UE_LOG(LogTemp,Warning,TEXT("triggerPos :%s , pow2 : %f"), *triggerPos.ToString(), pow2)

	return (pow2 <= spawnTriggerRadius * spawnTriggerRadius);
}

void ABattleFieldGameMode::SpawnEnemy()
{
	FVector spawnPosition;
	spawnPosition.X = FMath::RandRange(-spawnAreaMaximumRadius, spawnAreaMaximumRadius);
	spawnPosition.Y = FMath::RandRange(-spawnAreaMaximumRadius, spawnAreaMaximumRadius);
	spawnPosition.X = (spawnPosition.X < 0) ? FMath::Clamp<float>(spawnPosition.X, -spawnAreaMaximumRadius, -spawnAreaMinimumRadius) : FMath::Clamp<float>(spawnPosition.X, spawnAreaMinimumRadius, spawnAreaMaximumRadius);
	spawnPosition.Y = (spawnPosition.Y < 0) ? FMath::Clamp<float>(spawnPosition.Y, -spawnAreaMaximumRadius, -spawnAreaMinimumRadius) : FMath::Clamp<float>(spawnPosition.Y, spawnAreaMinimumRadius, spawnAreaMaximumRadius);
	spawnPosition.Z = 200.f;
	FVector myLocation = player->GetActorLocation();
	spawnPosition.X += myLocation.X;
	spawnPosition.Y += myLocation.Y;

	int count = 0;

	for (int i = 0; i < enemyPool.Num(); i++)
	{
		if (enemyPool[i]->IsHidden() && count < spawnEnmeyAICount)
		{
			//TODO:ReactiveAI 로 리팩토링 할것.
			enemyPool[i]->SetActorHiddenInGame(false);
			enemyPool[i]->SetActorEnableCollision(true);
			enemyPool[i]->SetActorLocation(spawnPosition);
			count++;
		}
	}

	//풀이 모자르면 추가적으로 생성해준다.
	if (count < spawnEnmeyAICount)
	{
		for (int i = 0; i < spawnEnmeyAICount - count; i++)
		{
			enemyPool.Add(GetWorld()->SpawnActor<AAICharacter>(enemyClass));
			enemyPool[enemyPool.Num() - 1]->SetActorLocation(spawnPosition);

			//다음에 스폰 해줄 애의 포지션을 지금 해준다.
			spawnPosition.X = FMath::RandRange(-spawnAreaMaximumRadius, spawnAreaMaximumRadius);
			spawnPosition.Y = FMath::RandRange(-spawnAreaMaximumRadius, spawnAreaMaximumRadius);
			spawnPosition.X = (spawnPosition.X < 0) ? FMath::Clamp<float>(spawnPosition.X, -spawnAreaMaximumRadius, -spawnAreaMinimumRadius) : FMath::Clamp<float>(spawnPosition.X, spawnAreaMinimumRadius, spawnAreaMaximumRadius);
			spawnPosition.Y = (spawnPosition.Y < 0) ? FMath::Clamp<float>(spawnPosition.Y, -spawnAreaMaximumRadius, -spawnAreaMinimumRadius) : FMath::Clamp<float>(spawnPosition.Y, spawnAreaMinimumRadius, spawnAreaMaximumRadius);
			spawnPosition.Z = 200.f;

			spawnPosition.X += myLocation.X;
			spawnPosition.Y += myLocation.Y;
		}
	}

	isSpawned = true;
	UE_LOG(LogTemp, Warning, TEXT("SpawnVector"));
}

void ABattleFieldGameMode::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	spawnCheckInterval = 1 / spawnCheckFrameRate;
	UE_LOG(LogTemp, Warning, TEXT("InitGameMOde"))
}

void ABattleFieldGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!isSpawned)
	{
		spawnCheckTimer += DeltaSeconds;
		if (spawnCheckTimer > spawnCheckInterval)
		{
			spawnCheckTimer = 0.f;
			if (IsInSpawnTrigger())
			{
				SpawnEnemy();
			}
		}
	}
}
