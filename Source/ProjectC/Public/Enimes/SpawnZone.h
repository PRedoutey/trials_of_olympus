// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTC_API SpawnZone
{
public:

	FVector Pos;
	float Radius;
	

	SpawnZone();
	~SpawnZone();
	
	
};

SpawnZone* createSpawner(FVector pos, float radius);
