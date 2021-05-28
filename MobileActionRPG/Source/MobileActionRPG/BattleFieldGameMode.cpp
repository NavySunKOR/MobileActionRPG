// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleFieldGameMode.h"
#include "MobileActionRPG/Player/PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

ABattleFieldGameMode::ABattleFieldGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
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
