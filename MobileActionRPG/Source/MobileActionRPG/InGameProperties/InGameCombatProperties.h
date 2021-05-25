// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

enum class StunType
{
	NoStun = 0,
	WeakStun = 1,
	MidStun,
	HeavyStun,
	VeryHeavyStun
};


class MOBILEACTIONRPG_API InGameCombatProperties
{
public:
	InGameCombatProperties();
	~InGameCombatProperties();
	static const float BACK_ATTACK_DAMAGE_MULTIPLYER;
	static const float SIDE_ATTACK_DAMAGE_MULTIPLYER;
	static const float PLAY_ANIMATION_SPEED_SLOWEST;
	static const float PLAY_ANIMATION_SPEED_SLOW;
	static const float PLAY_ANIMATION_SPEED_MID;
	static const float PLAY_ANIMATION_SPEED_FAST;
	static const float PLAY_ANIMATION_SPEED_FASTEST;
};
