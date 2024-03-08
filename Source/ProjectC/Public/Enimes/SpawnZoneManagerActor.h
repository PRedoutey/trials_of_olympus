// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnZone.h"
#include "Character/ProjectCCharacter.h"
#include "SpawnZoneManagerActor.generated.h"

UCLASS()
class PROJECTC_API ASpawnZoneManagerActor : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectCCharacter* Player;
	
	TArray<SpawnZone*> SpawnZoneList;
	SpawnZone* CurrentZone;
	
	float SpawnTimer;
	float SpawnDelay;
	float MinSpawnDelay;
	float SpawnDelayDecrement;
	float PlayerSafeDist;
	int SpawnBatchSize;
	int MaxEnemies;
	
	// Sets default values for this actor's properties
	ASpawnZoneManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LoadArray();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnCyclops(FVector location);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnSoldier(FVector location);
	
};
