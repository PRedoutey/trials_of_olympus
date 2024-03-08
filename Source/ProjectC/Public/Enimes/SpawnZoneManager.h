// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnZone.h"
#include "Character/ProjectCCharacter.h"
#include "Misc/FileHelper.h"

/**
 * 
 */
class PROJECTC_API SpawnZoneManager
{
public:
	AProjectCCharacter* Player;
	
	TArray<SpawnZone*> SpawnZoneList;
	SpawnZone* CurrentZone;
	
	float SpawnTimer;
	float SpawnDelay;
	float PlayerSafeDist;
	
	SpawnZoneManager();
	~SpawnZoneManager();
	
	void LoadArray();
	
	void Update(float dt);

	void Set_Player(AProjectCCharacter* player);
	
};
