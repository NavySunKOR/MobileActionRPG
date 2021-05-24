// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileActionRPG/AI/AICharacter.h"
#include "MobileActionRPG/InGameProperties/InGameCombatProperties.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
AttackDirectionType AAICharacter::CalcAttackDirection(FVector pAttackFrom)
{
	//좌우 45도, 총 90도 씩 4등분 하여 판정 처리를 함.


	FVector myLocation = GetActorLocation();
	myLocation.Z = 0.f;
	pAttackFrom.Z = 0.f;
	FVector dir = pAttackFrom - myLocation;
	FVector forward = GetActorForwardVector();
	/*
	0.7보다 크면 프론트
	0.7 보다 작고 -0.7보다 크면 45 ~135도 사이(사이드)
	-0.7보다 작으면 백
	*/
	float dotProductVal = FVector::DotProduct(forward, dir) / (forward.Size() * dir.Size());

	if (dotProductVal >= 0.7)
	{
		return AttackDirectionType::Front;
	}
	else if (dotProductVal <= -0.7)
	{
		return AttackDirectionType::Back;
	}
	else
	{
		FVector left = -GetActorRightVector();
		float dotProductLeftVal = FVector::DotProduct(left, dir) / (left.Size() * dir.Size());
		/*
		0.7보다 크면 왼쪽
		0.7 보다 작고 -0.7보다 크면 45 ~135도 사이(프론트, 백)
		-0.7보다 작으면 오른쪽
		*/
		if (dotProductLeftVal >= 0.7)
		{
			return AttackDirectionType::Left;
		}
		else if (dotProductLeftVal <= -0.7)
		{
			return AttackDirectionType::Right;
		}
		else
		{
			return AttackDirectionType::Front;
		}
	}
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	curHp = maxHp;

	UE_LOG(LogTemp, Warning, TEXT("init curHp %d"), curHp);
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::TookDamage(FVector pAttackFrom, float pOriginalDamage)
{
	UE_LOG(LogTemp, Warning, TEXT("before take hit curHp %d"), curHp);
	AttackDirectionType direction = CalcAttackDirection(pAttackFrom);
	if (direction == AttackDirectionType::Front)
	{
		curHp -= pOriginalDamage;
		UE_LOG(LogTemp,Warning, TEXT("curHp %d"),curHp);
	}
	else if (direction == AttackDirectionType::Back)
	{
		curHp -= pOriginalDamage * InGameCombatProperties::BACK_ATTACK_DAMAGE_MULTIPLYER;
		UE_LOG(LogTemp, Warning, TEXT("curHp %d"), curHp);
	}
	else if (direction == AttackDirectionType::Left)
	{
		curHp -= pOriginalDamage * InGameCombatProperties::SIDE_ATTACK_DAMAGE_MULTIPLYER;
		UE_LOG(LogTemp, Warning, TEXT("curHp %d"), curHp);
	}
	else if (direction == AttackDirectionType::Right)
	{
		curHp -= pOriginalDamage * InGameCombatProperties::SIDE_ATTACK_DAMAGE_MULTIPLYER;
		UE_LOG(LogTemp, Warning, TEXT("curHp %d"), curHp);
	}

	if (curHp <= 0)
	{
		curHp = 0;
		SetActorHiddenInGame(true);
	}
}

