// Fill out your copyright notice in the Description page of Project Settings.


#include "MobileActionRPG/AI/AICharacter.h"
#include "MobileActionRPG/InGameProperties/InGameCombatProperties.h"
#include <Kismet/GameplayStatics.h>

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
	trackingPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	mesh = GetMesh();
	defaultAnimationSpeed = InGameCombatProperties::PLAY_ANIMATION_SPEED_MID;
	mesh->SetPlayRate(defaultAnimationSpeed);

	UE_LOG(LogTemp, Warning, TEXT("init curHp %d"), curHp);
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isHit)
	{
		hitTimer += DeltaTime;
		if (hitTimer > hitInterval)
		{
			mesh->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
			mesh->SetPlayRate(defaultAnimationSpeed);
			isHit = false;
			hitTimer = 0.f;
		}
	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::TookDamage(FVector pAttackFrom, float pOriginalDamage, StunType pStunType)
{
	UE_LOG(LogTemp, Warning, TEXT("before take hit curHp %d"), curHp);
	if (isDead)
		return;

	AttackDirectionType direction = CalcAttackDirection(pAttackFrom);
	switch (direction)
	{
		case AttackDirectionType::Front: 
			curHp -= pOriginalDamage;
			break;
		case AttackDirectionType::Back: 
			curHp -= pOriginalDamage * InGameCombatProperties::BACK_ATTACK_DAMAGE_MULTIPLYER;
			break;
		case AttackDirectionType::Left:
			curHp -= pOriginalDamage * InGameCombatProperties::SIDE_ATTACK_DAMAGE_MULTIPLYER;
			break;
		case AttackDirectionType::Right: 
			curHp -= pOriginalDamage * InGameCombatProperties::SIDE_ATTACK_DAMAGE_MULTIPLYER;
			break;
		default: break;
	}

	if (pStunType != StunType::NoStun)
	{
		//mesh->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
		mesh->PlayAnimation(hitAnimations[(int)direction], false);
	}


	switch (pStunType)
	{
		case StunType::NoStun:
			mesh->SetPlayRate(InGameCombatProperties::PLAY_ANIMATION_SPEED_MID);
			hitInterval = hitAnimations[(int)direction]->SequenceLength * InGameCombatProperties::PLAY_ANIMATION_SPEED_MID;
			break;
		case StunType::WeakStun:
			mesh->SetPlayRate(InGameCombatProperties::PLAY_ANIMATION_SPEED_FAST);
			hitInterval = hitAnimations[(int)direction]->SequenceLength * InGameCombatProperties::PLAY_ANIMATION_SPEED_FAST;
			break;
		case StunType::MidStun:
			mesh->SetPlayRate(InGameCombatProperties::PLAY_ANIMATION_SPEED_MID);
			hitInterval = hitAnimations[(int)direction]->SequenceLength * InGameCombatProperties::PLAY_ANIMATION_SPEED_MID;
			break;
		case StunType::HeavyStun:
			mesh->SetPlayRate(InGameCombatProperties::PLAY_ANIMATION_SPEED_SLOW);
			hitInterval = hitAnimations[(int)direction]->SequenceLength * InGameCombatProperties::PLAY_ANIMATION_SPEED_SLOW;
			break;
		case StunType::VeryHeavyStun:
			mesh->SetPlayRate(InGameCombatProperties::PLAY_ANIMATION_SPEED_SLOWEST);
			hitInterval = hitAnimations[(int)direction]->SequenceLength * InGameCombatProperties::PLAY_ANIMATION_SPEED_SLOWEST;
			break;
		default:
			break;
	}


	if (curHp <= 0)
	{
		curHp = 0;
		isDead = true;
		mesh->PlayAnimation(deathAnimation, false);
		//SetActorHiddenInGame(true);
	}
}

