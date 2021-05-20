// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class USkeletalMeshComponent;


enum class CurrentAttackType
{
	Idle = 0,
	NormalAttack = 1,
	Skill1,
	Skill2,
	Skill3,
};

UCLASS()
class MOBILEACTIONRPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	bool isNormalAttackTransible = true;
	float attackTimer = 0.f;
	int normalAttackSequence = 0;
	USpringArmComponent* springArm;
	USkeletalMeshComponent* mesh;
	bool isLockOn = false;
	float moveHorizontal = 0.f;
	float moveVertical = 0.f;

	CurrentAttackType currentAttackType = CurrentAttackType::Idle;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimSequence*> normalAttackAnims;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimSequence*> skillAnims;

	UAnimInstance* animInst;


public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	void Jump();
	void Skill1();
	void Skill2();
	void Skill3();

	void MoveVertical(float pValue);
	void MoveHorizontal(float pValue);

	void RotateHorizontal(float pValue);
	void RotateVertical(float pValue);

	UFUNCTION(BlueprintPure)
	bool IsLockOn();

	UFUNCTION(BlueprintPure)
	float GetNonLockOnSpeed();
	UFUNCTION(BlueprintPure)
	float GetLockOnVertical();
	UFUNCTION(BlueprintPure)
	float GetLockOnHorizontal();
};
