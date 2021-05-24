// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TookDamage(FVector pAttackFrom,float pOriginalDamage);

};
