// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseEnemyController.h"
#include "SoldierController.generated.h"


UCLASS()
class PROJECTC_API ASoldierController : public ABaseEnemyController
{
	GENERATED_BODY()

	ASoldierController(const class FObjectInitializer& ObjectInitializer);
};
