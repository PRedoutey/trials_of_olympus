// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enimes/BaseEnemyController.h"
#include "GoblinController.generated.h"



UCLASS()
class PROJECTC_API AGoblinController : public ABaseEnemyController
{
	GENERATED_BODY()

	AGoblinController(const class FObjectInitializer& ObjectInitializer);
};

