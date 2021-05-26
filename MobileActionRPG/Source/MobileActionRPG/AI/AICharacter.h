// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MobileActionRPG/InGameProperties/InGameCombatProperties.h"
#include "MobileActionRPG/Player/PlayerCharacter.h"
#include "AICharacter.generated.h"

enum class AttackDirectionType
{
	Front = 0,
	Left = 1,
	Right = 2,
	Back = 3
};

class ACustomAIController;

UCLASS()
class MOBILEACTIONRPG_API AAICharacter : public ACharacter
{
	GENERATED_BODY()
private:
	int curHp;
	USkeletalMeshComponent* mesh;
	ACustomAIController* myController;
	float defaultAnimationSpeed;

	float hitTimer = 0.f;
	float attackTimer = 0.f;

	int selectedAttackSequences = 0;


	AttackDirectionType CalcAttackDirection(FVector pAttackFrom);


public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Status")
	int maxHp;
	UPROPERTY(EditAnywhere, Category = "Status")
	float attackRange;
	bool isHit = false;
	bool isAttack = false;
	bool isDead = false;
	bool isInRange = false;
	float attackInterval = 0.f;
	float hitInterval = 0.f;
	APlayerCharacter* trackingPlayer;

	UPROPERTY(EditAnywhere, Category = "Animations")
	TArray<UAnimSequence*> attackAnimations;
	UPROPERTY(EditAnywhere, Category = "Animations")
	TArray<UAnimSequence*> hitAnimations;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* deathAnimation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	void SetIsInRangeValue();
	void TookDamage(FVector pAttackFrom,float pOriginalDamage,StunType pStunType);

};
