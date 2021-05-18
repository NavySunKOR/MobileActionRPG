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
	springArm = Cast<USpringArmComponent>(GetDefaultSubobjectByName(TEXT("CameraArm")));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Skill2"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Skill3"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &APlayerCharacter::MoveVertical);
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &APlayerCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &APlayerCharacter::RotateVertical);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &APlayerCharacter::RotateHorizontal);
}


void APlayerCharacter::Attack()
{

}

void APlayerCharacter::Jump()
{

}


void APlayerCharacter::Skill1()
{

}


void APlayerCharacter::Skill2()
{

}

void APlayerCharacter::Skill3()
{

}

void APlayerCharacter::MoveVertical(float pValue)
{    
	if (pValue * pValue > 0 && GetVelocity().Size() <= 1)
	{
	    FRotator rot = GetActorRotation();
	    rot.Yaw = springArm->GetComponentRotation().Yaw;
	    SetActorRotation(rot);
		rot = springArm->GetComponentRotation();
		rot.Yaw = GetActorRotation().Yaw;
		springArm->SetWorldRotation(rot);

		UE_LOG(LogTemp, Warning, TEXT("GetActorYaw: %f SpringArmYaw: %f"), GetActorRotation().Yaw,springArm->GetTargetRotation().Yaw);
	}
	AddMovementInput(GetActorForwardVector()* pValue);
}

void APlayerCharacter::MoveHorizontal(float pValue)
{
	if (pValue * pValue > 0 && GetVelocity().Size() <= 1)
	{
		FRotator rot = GetActorRotation();
		rot.Yaw = springArm->GetComponentRotation().Yaw;
		SetActorRotation(rot);
		rot = springArm->GetComponentRotation();
		rot.Yaw = GetActorRotation().Yaw;
		springArm->SetWorldRotation(rot);

		UE_LOG(LogTemp, Warning, TEXT("GetActorYaw: %f SpringArmYaw: %f"), GetActorRotation().Yaw, springArm->GetTargetRotation().Yaw);
	}
	AddMovementInput(GetActorRightVector() * pValue);
}

void APlayerCharacter::RotateHorizontal(float pValue)
{    
	FRotator rot = springArm->GetComponentRotation();
	rot.Roll = 0.f;
	rot.Yaw += pValue;
	springArm->SetWorldRotation(rot);
}

void APlayerCharacter::RotateVertical(float pValue)
{
	FRotator rot = springArm->GetComponentRotation();
	rot.Roll = 0.f;
	rot.Pitch += pValue;
	springArm->SetWorldRotation(rot);
}