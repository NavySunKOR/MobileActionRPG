// Fill out your copyright notice in the Description page of Project Settings.

#include <GameFramework/SpringArmComponent.h>
#include "MobileActionRPG/Player/PlayerCharacter.h"

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
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�����̸� �����Ʈ
	float moveHorizontalEpsilon = moveHorizontal * moveHorizontal;
	float moveVerticalEpsilon = moveVertical * moveVertical;
	if (moveHorizontalEpsilon > 0 || moveVerticalEpsilon > 0)
	{
			//�����ϸ� ĳ���� ��ü�� �յ��¿�� ������.
			FRotator rotation(0, GetControlRotation().Yaw, 0);
			FVector directionForward = FRotationMatrix(rotation).GetUnitAxis(EAxis::X);
			FVector directionRight = FRotationMatrix(rotation).GetUnitAxis(EAxis::Y);
			FVector movementVector = directionForward * moveVertical + directionRight * moveHorizontal;
			float divivder = (moveHorizontalEpsilon > 0 && moveVerticalEpsilon > 0) ? 1.4f : 1.f;
			AddMovementInput(movementVector / divivder);
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
	UE_LOG(LogTemp,Warning,TEXT("Pyungta"))
}

void APlayerCharacter::Jump()
{

	UE_LOG(LogTemp, Warning, TEXT("Jump"))
}


void APlayerCharacter::Skill1()
{


	UE_LOG(LogTemp, Warning, TEXT("Skill1"))
}


void APlayerCharacter::Skill2()
{

	UE_LOG(LogTemp, Warning, TEXT("Skill2"))
}

void APlayerCharacter::Skill3()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill3"))
}

void APlayerCharacter::MoveVertical(float pValue)
{
	moveVertical = pValue;

}

void APlayerCharacter::MoveHorizontal(float pValue)
{
	moveHorizontal = pValue;
	//IsLockon�̳Ŀ� ���� ������ �޶��� .
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
