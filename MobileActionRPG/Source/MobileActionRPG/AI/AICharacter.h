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


UCLASS()
class MOBILEACTIONRPG_API AAICharacter : public ACharacter
{
	GENERATED_BODY()
private:
	int curHp;
	USkeletalMeshComponent* mesh;
	float defaultAnimationSpeed;
	bool isDead = false;

	bool isHit = false;
	float hitInterval = 0.f;
	float hitTimer = 0.f;

	AttackDirectionType CalcAttackDirection(FVector pAttackFrom);
	APlayerCharacter* trackingPlayer;


public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Status")
	int maxHp;

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

	void TookDamage(FVector pAttackFrom,float pOriginalDamage,StunType pStunType);

};
