// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class USkeletalMeshComponent;
class USkillInformation;


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
	USpringArmComponent* springArm;
	USkeletalMeshComponent* mesh;
	bool isLockOn = false;
	bool isNormalAttackTransible = true;
	float attackTimer = 0.f;
	float moveHorizontal = 0.f;
	float moveVertical = 0.f;

	int normalAttackSequence = 0;


	CurrentAttackType currentAttackType = CurrentAttackType::Idle;

	float skill1Timer = 0.f;
	float skill2Timer = 0.f;
	float skill3Timer = 0.f;


	UPROPERTY(EditAnywhere, Category = "NormalAttack")
	TArray<UAnimSequence*> normalAttackAnims;
	UPROPERTY(EditAnywhere, Category = "NormalAttack")
	UParticleSystem* normalAttackHitParticle;

	UPROPERTY(EditAnywhere, Category = "Skill1")
	float skill1CoolTime;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	float skill1Duration;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	UAnimSequence* skill1Anim;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	UParticleSystem* skill1HitParticle;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	USoundBase* skill1DurationEndSound;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	UParticleSystem* skill1DurationEndParticle;
	bool isSkill1Activated = false;
	float skill1DurationTimer;
	
	

	UPROPERTY(EditAnywhere, Category = "Skill2")
	float skill2CoolTime;
	UPROPERTY(EditAnywhere, Category = "Skill2")
	UAnimSequence* skill2Anim;
	UPROPERTY(EditAnywhere, Category = "Skill2")
	UParticleSystem* skill2HitParticle;

	UPROPERTY(EditAnywhere, Category = "Skill3")
	float skill3CoolTime;
	UPROPERTY(EditAnywhere, Category = "Skill3")
	UAnimSequence* skill3Anim;
	UPROPERTY(EditAnywhere, Category = "Skill3")
	UParticleSystem* skill3HitParticle;


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
