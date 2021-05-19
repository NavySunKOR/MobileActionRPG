// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class USkeletalMeshComponent;

UCLASS()
class MOBILEACTIONRPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY()
	USpringArmComponent* springArm;
	USkeletalMeshComponent* mesh;
	bool isLockOn = false;
	float moveHorizontal = 0.f;
	float moveVertical = 0.f;

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

};
