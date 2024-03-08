// Fill out your copyright notice in the Description page of Project Settings.


#include "Enimes/SpawnZone.h"

SpawnZone::SpawnZone()
{
}

SpawnZone::~SpawnZone()
{
}


SpawnZone* createSpawner(FVector pos, float radius)
{
	SpawnZone* newZone = new SpawnZone();
	newZone->Pos = pos;
	newZone->Radius = radius;
	return newZone;
}
