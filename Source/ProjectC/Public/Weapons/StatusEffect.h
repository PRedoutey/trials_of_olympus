// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Public/Enimes/BaseEnemy.h"
#include "../Public/Weapons/GunCore.h"

class AGunCore;
class AProjectCCharacter;

/**
 * 
 */
class PROJECTC_API StatusEffect
{
public:

	AGunCore* CoreInUse;
	ABaseEnemy* AfflictedEnemy;
	AProjectCCharacter* Player;

	bool isDead;


	// Pyro Attributes
	float dotTickDelay; // delay between ticks
	float dotTickTimer; // time left until next tick
	float dotLength; // time the dot effect is active
	float dotTimer; // time left for current dot effect
	float dotDamage; // damage per tick
	bool hasFirePool; // T/F if a fire pool has been spawned


	// Cryo Attributes
	float moveSpeedMultiplier; // scalar for how much an enemy is slowed
	float slowedLength; // time the slowing effect is active
	float slowedTimer; // time left for current slowing effect
	float baseEnemyMoveSpeed;



	// Electro Attributes
	float chainDistance;
	float chainDamage;
	
	
	
	
	StatusEffect();
	
	~StatusEffect();

	void Die();

	void Set_Core(AGunCore* core);

	void Set_Enemy(ABaseEnemy* enemy);

	void Set_Player(AProjectCCharacter* player);

	void OnApply();

	void Freeze(ABaseEnemy* enemy, bool guaranteed);

	bool Update(float DeltaTime);
	
};



