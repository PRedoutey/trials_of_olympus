// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "Cyclops.generated.h"

UCLASS()
class PROJECTC_API ACyclops : public ABaseEnemy
{
	GENERATED_BODY()

	UAnimMontage* DeathAnim;
	UAnimMontage* AttackAnim;
	UAnimMontage* ThrowAnim;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
protected:
	virtual void Die() override;
	
public:
	// Sets default values for this character's properties
	ACyclops(const FObjectInitializer& ObjectInitializer);

	// This is the C++ code required to call a blueprint function in C++, kept for posterity.
	/*UFUNCTION(BlueprintNativeEvent)
	void PlayDeathAnim();

	void PlayDeathAnim_Implementation();*/
};

