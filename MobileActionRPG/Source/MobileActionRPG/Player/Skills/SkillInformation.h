// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillInformation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class MOBILEACTIONRPG_API USkillInformation : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> skillSounds;
	UPROPERTY(EditAnywhere, Category = "Particle")
	TArray<UParticleSystem*> skillParticles;
	UPROPERTY(EditAnywhere, Category = "Particle")
	UParticleSystem* skillHitParticle;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence* skillAnimation;

};
