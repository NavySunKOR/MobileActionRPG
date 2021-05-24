// Fill out your copyright notice in the Description page of Project Settings.

#include "MobileActionRPG/Player/PlayerCharacter.h"
#include "MobileActionRPG/AI/AICharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
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
	skill1Timer = skill1CoolTime;
	skill2Timer = skill2CoolTime;
	skill3Timer = skill3CoolTime;
	skill1DurationHitInterval = (skill1Duration / skill1DurationHitCount);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//
	float moveHorizontalEpsilon = moveHorizontal * moveHorizontal;
	float moveVerticalEpsilon = moveVertical * moveVertical;
	if ((moveHorizontalEpsilon > 0 || moveVerticalEpsilon > 0) && currentAttackType == CurrentAttackType::Idle)
	{
			//
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
			if (attackTimer > skill1Anim->SequenceLength)
			{
				attackTimer = 0.f;
				currentAttackType = CurrentAttackType::Idle;
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}
		}
		else if (currentAttackType == CurrentAttackType::Skill2)
		{
			if (attackTimer > skill2Anim->SequenceLength)
			{
				attackTimer = 0.f;
				currentAttackType = CurrentAttackType::Idle;
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}
		}
		else if (currentAttackType == CurrentAttackType::Skill3)
		{
			if (attackTimer > skill3Anim->SequenceLength)
			{
				attackTimer = 0.f;
				currentAttackType = CurrentAttackType::Idle;
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}
		}
	}

	//Cooltime
	if (skill1Timer < skill1CoolTime)
	{
		skill1Timer += DeltaTime;
	}

	if (skill2Timer < skill2CoolTime)
	{
		skill2Timer += DeltaTime;
	}

	if (skill3Timer < skill3CoolTime)
	{
		skill3Timer += DeltaTime;
	}

	//Skill1Actiavtion

	if (isSkill1Activated)
	{
		skill1DurationTimer += DeltaTime;
		skill1DurationHitTimer += DeltaTime;

		if (skill1DurationHitTimer > skill1DurationHitInterval)
		{
			skill1DurationHitTimer = 0.f;
			Skill1CheckHit(Skill1AttackStatus::Duration);
		}

		//Skill1 duration 판정
		if (skill1DurationTimer > skill1Duration)
		{
			//Skill1 finish 판정
			skill1DurationTimer = 0.f;
			isSkill1Activated = false;
			Skill1CheckHit(Skill1AttackStatus::End);
			UGameplayStatics::SpawnEmitterAttached(skill1DurationEndParticle, GetMesh(), TEXT("Root"),FVector(0,0,100.f));
			UGameplayStatics::SpawnSoundAttached(skill1DurationEndSound, GetMesh(), TEXT("Root"));
		}
	}

	//Jump
	if (isJump)
	{
		if (!IsInAir())
		{
			isJump = false;
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
	if (!IsInAir())
	{
		if (currentAttackType == CurrentAttackType::Idle)
		{
			currentAttackType = CurrentAttackType::NormalAttack;
			isNormalAttackTransible = false;
			mesh->PlayAnimation(normalAttackAnims[normalAttackSequence], false);
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
}

void APlayerCharacter::AttackCheckHit()
{

}

void APlayerCharacter::Jump()
{
	isJump = true;
	GetCharacterMovement()->DoJump(false);
}


void APlayerCharacter::Skill1()
{
	if ((currentAttackType == CurrentAttackType::Idle || currentAttackType == CurrentAttackType::NormalAttack) 
		&& skill1Timer >= skill1CoolTime
		&& !IsInAir())
	{
		skill1Timer = 0.f;
		currentAttackType = CurrentAttackType::Skill1;
		isSkill1Activated = true;
		mesh->PlayAnimation(skill1Anim, false);
		Skill1CheckHit(Skill1AttackStatus::Init);
		UE_LOG(LogTemp, Warning, TEXT("Skill1"))
	}
}

void APlayerCharacter::Skill1CheckHit(Skill1AttackStatus pStatus)
{
	FVector myLocation = GetActorLocation();
	TActorRange<AAICharacter> aiCharacters = TActorRange<AAICharacter>(GetWorld());
	float rangeMultiplyRatio = (pStatus == Skill1AttackStatus::Init || pStatus == Skill1AttackStatus::End) ? 1.3f : 1.f;
	for (AAICharacter* aiCharacter : aiCharacters)
	{
		if (!aiCharacter->IsHidden())
		{
			FVector vectorDiff = aiCharacter->GetActorLocation() - GetActorLocation();
			float distance = vectorDiff.Size();
			float heightDistance = FMath::Abs<float>(vectorDiff.Z);

			if (distance < skill1Radius * rangeMultiplyRatio && heightDistance < 50.f)
			{
				if (pStatus == Skill1AttackStatus::Init)
				{
					aiCharacter->TookDamage(GetActorLocation(), skill1InitDamageMultiplier * damage);
				}
				else if (pStatus == Skill1AttackStatus::Duration)
				{
					aiCharacter->TookDamage(GetActorLocation(), skill1DurationDamageMultiplier * damage);
				}
				else
				{
					aiCharacter->TookDamage(GetActorLocation(), skill1EndDamageMultiplier * damage);
				}
			}
		}
	}

}


void APlayerCharacter::Skill2()
{
	if (currentAttackType == CurrentAttackType::Idle 
		&& skill2Timer >= skill2CoolTime
		&& !IsInAir())
	{
		skill2Timer = 0.f;
		currentAttackType = CurrentAttackType::Skill2;
		mesh->PlayAnimation(skill2Anim, false);
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 1000.f, true);
		GetCharacterMovement()->DoJump(false);
		UE_LOG(LogTemp, Warning, TEXT("Skill2"))
	}
}

void APlayerCharacter::Skill2CheckHit()
{
}

void APlayerCharacter::Skill3()
{
	if (currentAttackType == CurrentAttackType::Idle 
		&& skill3Timer >= skill3CoolTime
		&& !IsInAir())
	{
		skill3Timer = 0.f;
		currentAttackType = CurrentAttackType::Skill3;
		mesh->PlayAnimation(skill3Anim, false);
		UE_LOG(LogTemp, Warning, TEXT("Skill3"))
	}
}

void APlayerCharacter::Skill3CheckHit()
{
}

void APlayerCharacter::MoveVertical(float pValue)
{
	moveVertical = pValue;

}

void APlayerCharacter::MoveHorizontal(float pValue)
{
	moveHorizontal = pValue;
	//IsLockon
}

void APlayerCharacter::RotateHorizontal(float pValue)
{   
	AddControllerYawInput(pValue);
}

void APlayerCharacter::RotateVertical(float pValue)
{
	AddControllerPitchInput(pValue);
}

bool APlayerCharacter::IsJump()
{
	return isJump;
}

bool APlayerCharacter::IsInAir()
{
	return GetCharacterMovement()->IsFalling();
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
