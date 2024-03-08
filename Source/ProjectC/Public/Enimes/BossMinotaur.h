// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "BossMinotaur.generated.h"

UCLASS()
class PROJECTC_API ABossMinotaur : public ABaseEnemy
{
	GENERATED_BODY()

	UAnimMontage* DeathAnim;
	UAnimMontage* AttackAnim2;
	UAnimMontage* AttackAnim3;

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
    virtual void Die() override;

public:
	// Sets default values for this character's properties
	ABossMinotaur(const FObjectInitializer& ObjectInitializer);
};

