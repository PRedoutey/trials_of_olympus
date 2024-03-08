
// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enimes/EnemyHealthBar.h"

void UEnemyHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);



	if(!OwningEnemy.IsValid())
		return;
	
	if (spawnVisFlag)
	{
		OldHealth = OwningEnemy->MaxHealth;
		HealthBar->SetVisibility(ESlateVisibility::Hidden);
		spawnVisFlag = 0;
	}

	

	if(OwningEnemy->Health > 1)
	{

		HealthBar->SetPercent( OwningEnemy->Health / OwningEnemy->MaxHealth);
		
		//took damage, draw hp bar and start countdown
		if (OwningEnemy->Health <  OldHealth )
			timer = 2.0f;
		if (timer > 0.0f)
		{
			HealthBar->SetVisibility(ESlateVisibility::Visible);
			timer -= InDeltaTime;
		}
		else 
			HealthBar->SetVisibility(ESlateVisibility::Hidden);
		
		//update health to detect new damage
		OldHealth = OwningEnemy->Health;


			
		

	}
	else //if dead, don't work immediately
	{
		HealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}



