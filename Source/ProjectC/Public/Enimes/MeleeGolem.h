// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "MeleeGolem.generated.h"

UCLASS()
class PROJECTC_API AMeleeGolem : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMeleeGolem(const FObjectInitializer& ObjectInitializer);
};
