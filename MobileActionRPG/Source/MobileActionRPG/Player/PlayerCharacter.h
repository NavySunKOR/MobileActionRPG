// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"	
#include "EngineUtils.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class USkeletalMeshComponent;
class USkillInformation;
class AAICharacter;


enum class CurrentAttackType
{
	Idle = 0,
	NormalAttack = 1,
	Skill1,
	Skill2,
	Skill3,
};

enum class Skill1AttackStatus
{
	Init = 1,
	Duration = 2,
	End = 3
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
	bool isJump = false;
	float attackTimer = 0.f;
	float moveHorizontal = 0.f;
	float moveVertical = 0.f;

	int normalAttackSequence = 0;


	CurrentAttackType currentAttackType = CurrentAttackType::Idle;

	float skill1Timer = 0.f;
	float skill2Timer = 0.f;
	float skill3Timer = 0.f;


	bool isActiveSkillBounded = false;

	UPROPERTY(EditAnywhere, Category = "Status")
	float damage;


	UPROPERTY(EditAnywhere, Category = "NormalAttack")
	float normalAttackWeakDamageMultiplier;
	UPROPERTY(EditAnywhere, Category = "NormalAttack")
	float normalAttackStrongDamageMultiplier;
	UPROPERTY(EditAnywhere, Category = "NormalAttack")
	float normalAttackRadius;
	UPROPERTY(EditAnywhere, Category = "NormalAttack")
	TArray<UAnimSequence*> normalAttackAnims;
	UPROPERTY(EditAnywhere, Category = "NormalAttack")
	UParticleSystem* normalAttackHitParticle;

	UPROPERTY(EditAnywhere, Category = "Skill1")
	float skill1InitDamageMultiplier;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	float skill1DurationDamageMultiplier;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	float skill1EndDamageMultiplier;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	float skill1CoolTime;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	float skill1Duration;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	int skill1DurationHitCount;
	UPROPERTY(EditAnywhere, Category = "Skill1")
	float skill1Radius;
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
	float skill1DurationHitTimer;
	float skill1DurationHitInterval;
	

	UPROPERTY(EditAnywhere, Category = "Skill2")
	float skill2AttackRadius;
	UPROPERTY(EditAnywhere, Category = "Skill2")
	float skill2DamageMultiplier;
	UPROPERTY(EditAnywhere, Category = "Skill2")
	float skill2CoolTime;
	UPROPERTY(EditAnywhere, Category = "Skill2")
	UAnimSequence* skill2Anim;
	UPROPERTY(EditAnywhere, Category = "Skill2")
	UParticleSystem* skill2HitParticle;

	UPROPERTY(EditAnywhere, Category = "Skill3")
	float skill3AttackRadius;
	UPROPERTY(EditAnywhere, Category = "Skill3")
	float skill3DamageMultiplier;
	UPROPERTY(EditAnywhere, Category = "Skill3")
	float skill3CoolTime;
	UPROPERTY(EditAnywhere, Category = "Skill3")
	UAnimSequence* skill3Anim;
	UPROPERTY(EditAnywhere, Category = "Skill3")
	UParticleSystem* skill3HitParticle;

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
	void AttackCheckHit();
	void Jump();
	void Skill1();
	void Skill1CheckHit(Skill1AttackStatus pStatus);
	void Skill2();
	void Skill2CheckHit();
	void Skill3();
	void Skill3CheckHit();

	void MoveVertical(float pValue);
	void MoveHorizontal(float pValue);

	void RotateHorizontal(float pValue);
	void RotateVertical(float pValue);

	UFUNCTION(BlueprintPure)
	bool IsJump();
	UFUNCTION(BlueprintPure)
	bool IsInAir();

	UFUNCTION(BlueprintPure)
	bool IsLockOn();
	UFUNCTION(BlueprintPure)
	float GetNonLockOnSpeed();
	UFUNCTION(BlueprintPure)
	float GetLockOnVertical();
	UFUNCTION(BlueprintPure)
	float GetLockOnHorizontal();
};
