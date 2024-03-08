// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/Weapons/GunCore.h"
#include "FirePoolActor.generated.h"

class GunCore;

UCLASS()
class PROJECTC_API AFirePoolActor : public AActor
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties
	AFirePoolActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGunCore* ParentCore;

	float lifetime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UFUNCTION(BlueprintCallable)
	void SetCore(AGunCore* core);

};
