// Fill out your copyright notice in the Description page of Project Settings.

#include "MobileActionRPG/Player/PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Animation/AnimMontage.h>


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	mesh = GetMesh();
	animInst = mesh->GetAnimInstance();
	springArm = Cast<USpringArmComponent>(GetDefaultSubobjectByName(TEXT("CameraArm")));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//움직이면 무브먼트
	float moveHorizontalEpsilon = moveHorizontal * moveHorizontal;
	float moveVerticalEpsilon = moveVertical * moveVertical;
	if ((moveHorizontalEpsilon > 0 || moveVerticalEpsilon > 0) && currentAttackType == CurrentAttackType::Idle)
	{
			//락온하면 캐릭터 자체가 앞뒤좌우로 움직임.
			FRotator rotation(0, GetControlRotation().Yaw, 0);
			FVector directionForward = FRotationMatrix(rotation).GetUnitAxis(EAxis::X);
			FVector directionRight = FRotationMatrix(rotation).GetUnitAxis(EAxis::Y);
			FVector movementVector = directionForward * moveVertical + directionRight * moveHorizontal;
			float divivder = (moveHorizontalEpsilon > 0 && moveVerticalEpsilon > 0) ? 1.4f : 1.f;
			AddMovementInput(movementVector / divivder);
	}

	if (currentAttackType != CurrentAttackType::Idle)
	{
		attackTimer += DeltaTime;
		if (currentAttackType == CurrentAttackType::NormalAttack)
		{
			int calc = (normalAttackSequence - 1 < 0) ? normalAttackAnims.Num() - 1 : normalAttackSequence - 1;
			if (attackTimer > normalAttackAnims[calc]->SequenceLength * 0.6f)
			{
				isNormalAttackTransible = true;
			}

			if (attackTimer > normalAttackAnims[calc]->SequenceLength)
			{
				currentAttackType = CurrentAttackType::Idle;
				attackTimer = 0.f;
				normalAttackSequence = 0;
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}
		}
		else if (currentAttackType == CurrentAttackType::Skill1)
		{
			if (attackTimer > skillAnims[0]->SequenceLength)
			{
				currentAttackType = CurrentAttackType::Idle;
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}
		}
		else if (currentAttackType == CurrentAttackType::Skill1)
		{
			if (attackTimer > skillAnims[1]->SequenceLength)
			{
				currentAttackType = CurrentAttackType::Idle;
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}
		}
		else if (currentAttackType == CurrentAttackType::Skill1)
		{
			if (attackTimer > skillAnims[2]->SequenceLength)
			{
				currentAttackType = CurrentAttackType::Idle;
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}
		}
	}


}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill1);
	PlayerInputComponent->BindAction(TEXT("Skill2"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill2);
	PlayerInputComponent->BindAction(TEXT("Skill3"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill3);
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &APlayerCharacter::MoveVertical);
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &APlayerCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &APlayerCharacter::RotateVertical);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &APlayerCharacter::RotateHorizontal);
}


void APlayerCharacter::Attack()
{
	if (currentAttackType == CurrentAttackType::Idle)
	{
		currentAttackType = CurrentAttackType::NormalAttack;
		isNormalAttackTransible = false;
		mesh->PlayAnimation(normalAttackAnims[normalAttackSequence],false);
		normalAttackSequence++;
		if (normalAttackSequence >= normalAttackAnims.Num())
		{
			normalAttackSequence = 0;
		}
		UE_LOG(LogTemp, Warning, TEXT("Pyungta"))
	}
	else if (currentAttackType == CurrentAttackType::NormalAttack && isNormalAttackTransible)
	{
		attackTimer = 0.f;
		isNormalAttackTransible = false;
		mesh->PlayAnimation(normalAttackAnims[normalAttackSequence], false);
		normalAttackSequence++;
		if (normalAttackSequence >= normalAttackAnims.Num())
		{
			normalAttackSequence = 0;
		}
		UE_LOG(LogTemp, Warning, TEXT("Pyungta Sequence"))
	}
}

void APlayerCharacter::Jump()
{

	UE_LOG(LogTemp, Warning, TEXT("Jump"))
}


void APlayerCharacter::Skill1()
{
	if (currentAttackType == CurrentAttackType::Idle)
	{
		currentAttackType = CurrentAttackType::Skill1;
		mesh->PlayAnimation(skillAnims[0], false);
		UE_LOG(LogTemp, Warning, TEXT("Skill1"))
	}
}


void APlayerCharacter::Skill2()
{
	if (currentAttackType == CurrentAttackType::Idle)
	{
		currentAttackType = CurrentAttackType::Skill2;
		mesh->PlayAnimation(skillAnims[1], false);
		UE_LOG(LogTemp, Warning, TEXT("Skill1"))
	}
	UE_LOG(LogTemp, Warning, TEXT("Skill2"))
}

void APlayerCharacter::Skill3()
{
	if (currentAttackType == CurrentAttackType::Idle)
	{
		currentAttackType = CurrentAttackType::Skill3;
		mesh->PlayAnimation(skillAnims[2], false);
		UE_LOG(LogTemp, Warning, TEXT("Skill1"))
	}
	UE_LOG(LogTemp, Warning, TEXT("Skill3"))
}

void APlayerCharacter::MoveVertical(float pValue)
{
	moveVertical = pValue;

}

void APlayerCharacter::MoveHorizontal(float pValue)
{
	moveHorizontal = pValue;
	//IsLockon이냐에 따라서 조작이 달라짐 .
}

void APlayerCharacter::RotateHorizontal(float pValue)
{   
	AddControllerYawInput(pValue);
}

void APlayerCharacter::RotateVertical(float pValue)
{
	AddControllerPitchInput(pValue);
}

bool APlayerCharacter::IsLockOn()
{
	return isLockOn;
}

float APlayerCharacter::GetNonLockOnSpeed()
{
	float value = 0;
	float moveHorizontalEpsilon = moveHorizontal * moveHorizontal;
	float moveVerticalEpsilon = moveVertical * moveVertical;
	if (moveHorizontalEpsilon > 0 && moveVerticalEpsilon > 0)
	{
		value = FMath::Sqrt((moveHorizontalEpsilon + moveVerticalEpsilon)) / 1.4f;
	}
	else
	{
		value = (moveHorizontalEpsilon > 0) ? moveHorizontal : moveVertical;
	}
	if (value < 0)
	{
		value = -value;
	}

	return value;
}

float APlayerCharacter::GetLockOnVertical()
{
	return moveVertical;
}

float APlayerCharacter::GetLockOnHorizontal()
{
	return moveHorizontal;
}
