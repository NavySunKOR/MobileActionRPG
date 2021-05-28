// Fill out your copyright notice in the Description page of Project Settings.

#include "MobileActionRPG/Player/PlayerCharacter.h"
#include "MobileActionRPG/AI/AICharacter.h"
#include "MobileActionRPG/InGameProperties/InGameCombatProperties.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Animation/AnimMontage.h>


void APlayerCharacter::LookTarget()
{
	TActorRange<AAICharacter> aiCharacters = TActorRange<AAICharacter>(GetWorld());
	float shortest = 0.f;
	FVector targetLocation;
	bool isLocationSet = false;

	for (AAICharacter* aiCharacter : aiCharacters)
	{
		float distance = (aiCharacter->GetActorLocation() - GetActorLocation()).Size();

		if (!aiCharacter->IsHidden() && (shortest <= 0.f ||distance < shortest))
		{
			shortest = distance;
			targetLocation = aiCharacter->GetActorLocation();
			isLocationSet = true;
		}
	}

	if (isLocationSet)
	{
		FVector dir = (targetLocation - GetActorLocation());
		SetActorRotation(FMath::Lerp(GetActorRotation(), dir.Rotation(), 0.1f));
	}
}

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
	springArm = Cast<USpringArmComponent>(GetDefaultSubobjectByName(TEXT("CameraArm")));
	skill1Timer = skill1CoolTime;
	skill2Timer = skill2CoolTime;
	skill3Timer = skill3CoolTime;
	skill1DurationHitInterval = (skill1Duration / skill1DurationHitCount);

	UE_LOG(LogTemp, Warning, TEXT("Pyungta dmg %f"), damage);
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
		LookTarget();
		if (currentAttackType == CurrentAttackType::NormalAttack)
		{
			int calc = (normalAttackSequence - 1 < 0) ? normalAttackAnims.Num() - 1 : normalAttackSequence - 1;
			if (attackTimer > normalAttackAnims[calc]->SequenceLength * 0.6f)
			{
				isNormalAttackTransible = true;
			}

			if (attackTimer > normalAttackAnims[calc]->SequenceLength)
			{
				attackTimer = 0.f;
				normalAttackSequence = 0;
				currentAttackType = CurrentAttackType::Idle;
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
			GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 10.f, true);
			if (attackTimer > skill2Anim->SequenceLength * 0.7f && !isActiveSkillBounded)
			{
				Skill2CheckHit();
				isActiveSkillBounded = true;
			}
			if (attackTimer > skill2Anim->SequenceLength)
			{
				isActiveSkillBounded = false;
				attackTimer = 0.f;
				currentAttackType = CurrentAttackType::Idle;
				mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}
		}
		else if (currentAttackType == CurrentAttackType::Skill3)
		{
			if (attackTimer > skill3Anim->SequenceLength * 0.7f && !isActiveSkillBounded)
			{
				Skill3CheckHit();
				isActiveSkillBounded = true;
			}
			if (attackTimer > skill3Anim->SequenceLength)
			{
				isActiveSkillBounded = false;
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
			AttackCheckHit();
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
			AttackCheckHit();
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
	FVector myLocation = GetActorLocation();
	TActorRange<AAICharacter> aiCharacters = TActorRange<AAICharacter>(GetWorld());

	for (AAICharacter* aiCharacter : aiCharacters)
	{
		if (!aiCharacter->IsHidden())
		{

			UE_LOG(LogTemp, Warning, TEXT("AttackHit Check"))
			FVector vectorDiff = aiCharacter->GetActorLocation() - GetActorLocation();
			float distance = vectorDiff.Size();
			float rangeMultiplyRatio = (normalAttackSequence == 1 || normalAttackSequence == 3) ? 1.3f : 1.f;

			UE_LOG(LogTemp, Warning, TEXT("Property Check distance : %f , vectorDiff.Z  %f"), distance, vectorDiff.Z);

			if (distance < normalAttackRadius * rangeMultiplyRatio
				&&
				(vectorDiff.Z >= 0 && vectorDiff.Z <= 50.f))
			{
				FVector forward = GetActorForwardVector();
				vectorDiff.Z = 0.f;
				forward.Z = 0.f;
				
				//평타 판정은 180도이다. 그러므로 굳이 cos 값이 필요 없다.
				if (FVector::DotProduct(forward, vectorDiff) > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("GO gym go Check"))
					aiCharacter->TookDamage(GetActorLocation(), damage * (normalAttackSequence == 1 || normalAttackSequence == 3) ? normalAttackStrongDamageMultiplier : normalAttackWeakDamageMultiplier,StunType::WeakStun);
				}
			}
		}
	}
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
		LookTarget();
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
				switch (pStatus)
				{
				case Skill1AttackStatus::Init:
					aiCharacter->TookDamage(GetActorLocation(), skill1InitDamageMultiplier * damage, StunType::MidStun);
					break;
				case Skill1AttackStatus::Duration:
					aiCharacter->TookDamage(GetActorLocation(), skill1DurationDamageMultiplier * damage, StunType::NoStun);
					break;
				default:
					aiCharacter->TookDamage(GetActorLocation(), skill1EndDamageMultiplier * damage, StunType::MidStun);
					break;
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
		LookTarget();
		skill2Timer = 0.f;
		currentAttackType = CurrentAttackType::Skill2;
		mesh->PlayAnimation(skill2Anim, false);
		GetCharacterMovement()->DoJump(false);
		UE_LOG(LogTemp, Warning, TEXT("Skill2"))
	}
}

void APlayerCharacter::Skill2CheckHit()
{
	FVector myLocation = GetActorLocation();
	TActorRange<AAICharacter> aiCharacters = TActorRange<AAICharacter>(GetWorld());

	for (AAICharacter* aiCharacter : aiCharacters)
	{
		if (!aiCharacter->IsHidden())
		{

			UE_LOG(LogTemp, Warning, TEXT("AttackHit Check"))
			FVector vectorDiff = aiCharacter->GetActorLocation() - GetActorLocation();
			float distance = vectorDiff.Size();

			UE_LOG(LogTemp, Warning, TEXT("Property Check distance : %f , vectorDiff.Z  %f"), distance, vectorDiff.Z);

			if (distance < skill2AttackRadius 
				&&
				(vectorDiff.Z >= 0 && vectorDiff.Z <= 50.f))
			{
				FVector forward = GetActorForwardVector();
				vectorDiff.Z = 0.f;
				forward.Z = 0.f;

				//스킬 판정은 60도이다. 
				if (FVector::DotProduct(forward, vectorDiff) / (forward.Size() * vectorDiff.Size())> 0.8f)
				{
					UE_LOG(LogTemp, Warning, TEXT("Skill go gym go total %f , damage : %f , multiply :%f"), damage * skill2DamageMultiplier,damage, skill2DamageMultiplier)
					aiCharacter->TookDamage(GetActorLocation(), damage * skill2DamageMultiplier, StunType::HeavyStun);
				}
			}
		}
	}
}

void APlayerCharacter::Skill3()
{
	if (currentAttackType == CurrentAttackType::Idle 
		&& skill3Timer >= skill3CoolTime
		&& !IsInAir())
	{
		LookTarget();
		skill3Timer = 0.f;
		currentAttackType = CurrentAttackType::Skill3;
		mesh->PlayAnimation(skill3Anim, false);
		UE_LOG(LogTemp, Warning, TEXT("Skill3"))
	}
}

void APlayerCharacter::Skill3CheckHit()
{
	FVector myLocation = GetActorLocation();
	TActorRange<AAICharacter> aiCharacters = TActorRange<AAICharacter>(GetWorld());

	for (AAICharacter* aiCharacter : aiCharacters)
	{
		if (!aiCharacter->IsHidden())
		{

			UE_LOG(LogTemp, Warning, TEXT("Skill3CheckHit Check"))
			FVector vectorDiff = aiCharacter->GetActorLocation() - GetActorLocation();
			float distance = vectorDiff.Size();

			//판정은 위아래로 70.f 에 원형으로 데미지가 들어감.
			if (distance < skill3AttackRadius
				&&FMath::Abs<float>(vectorDiff.Z)<= 110.f)
			{
				aiCharacter->TookDamage(GetActorLocation(), damage * skill3DamageMultiplier, StunType::VeryHeavyStun);
			}
		}
	}

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
