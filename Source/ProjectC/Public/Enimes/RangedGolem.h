// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "RangedGolem.generated.h"

UCLASS()
class PROJECTC_API ARangedGolem : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARangedGolem(const FObjectInitializer& ObjectInitializer);
};
