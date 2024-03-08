// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "../Public/Enimes/BaseEnemy.h"
#include "../Public/Enimes/RangedGolem.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTC_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	

	public:
	TWeakObjectPtr<ABaseEnemy> OwningEnemy;
	TWeakObjectPtr<ARangedGolem> OwningEnemyR;
	float timer = 0.0f;
	float OldHealth = 100.0f;
	float spawnVisFlag = 1;

		//set to enemy parent class
	void SetOwner( ABaseEnemy* InEnemy) { OwningEnemy = InEnemy ;}
	void SetOwnerR(ARangedGolem* InEnemy) { OwningEnemyR = InEnemy; }

	void NativeTick ( const FGeometry& MyGeometry, float InDeltaTime ) override;
	
	

	UPROPERTY(meta = (BindWidget))
	 UProgressBar* HealthBar;
	
};
