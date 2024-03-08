// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enimes/BaseEnemy.h"
#include "Goblin.generated.h"

UCLASS()
class PROJECTC_API AGoblin : public ABaseEnemy
{
	GENERATED_BODY()

	UAnimMontage* DeathAnim;
	
public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void Die() override;
	
public:
	// Sets default values for this character's properties
	AGoblin(const FObjectInitializer& ObjectInitializer);
};
