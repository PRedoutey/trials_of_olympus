// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "Generator.generated.h"

UCLASS()
class PROJECTC_API AGenerator : public AActor
{
	GENERATED_BODY()

protected:
	int MaxCount;
	int CurCount;

public:	
	bool isActive;
	float MaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* MyGenerator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ADeathDust;

	
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddCount();

	UFUNCTION(BlueprintCallable)
	bool GetIsActive();

	UFUNCTION(BLueprintCallable)
	void SetIsActive(bool Active);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnGoblin(FVector location);
};
