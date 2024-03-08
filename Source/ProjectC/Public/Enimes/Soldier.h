// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "Soldier.generated.h"

UCLASS()
class PROJECTC_API ASoldier : public ABaseEnemy
{
	GENERATED_BODY()

	UAnimMontage* DeathAnim;
	UAnimMontage* AttackAnim1;
	UAnimMontage* AttackAnim2;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Character)
	bool EndTracking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Character)
    bool EndMovement = false;

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void Die() override;

public:
	// Sets default values for this character's properties
	ASoldier(const FObjectInitializer& ObjectInitializer);
};