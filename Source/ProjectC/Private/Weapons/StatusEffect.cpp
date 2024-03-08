// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/StatusEffect.h"

#include "Character/ProjectCCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"


StatusEffect::StatusEffect()
{
	isDead = false;

	
	// Pyro
	dotTickDelay = 0.25;
	dotTickTimer = 0;
	dotLength = 5;
	dotTimer = 0;
	dotDamage = 10.0;
	hasFirePool = false;

	// Cryo
	moveSpeedMultiplier = 0.5;
	slowedLength = 5;
	slowedTimer = 0;
	baseEnemyMoveSpeed = 0;

	// Elctro
	chainDistance = 700;
	chainDamage = 5;
	
}

StatusEffect::~StatusEffect()
{
	
}

void StatusEffect::Die()
{
	AfflictedEnemy->IsAffected = false;
	Player->Afflictions.Remove(this);
	delete this;
}

void StatusEffect::Set_Core(AGunCore* core)
{
	CoreInUse = core;
}

void StatusEffect::Set_Enemy(ABaseEnemy* enemy)
{
	AfflictedEnemy = enemy;
}

void StatusEffect::Set_Player(AProjectCCharacter* player)
{
	Player = player;
}


void StatusEffect::OnApply()
{
	TArray<AActor*> FoundActors;
	FVector AfflictedEnemyLocation = AfflictedEnemy->GetActorLocation();
	switch (CoreInUse->CurrentElement)
	{
	case Pyro:
		dotTickTimer = dotTickDelay * CoreInUse->dotTickModifier;
		dotTimer = dotLength;
		break;
	case Cryo:
		baseEnemyMoveSpeed = AfflictedEnemy->MoveSpeed();
		AfflictedEnemy->MoveSpeed() *= (moveSpeedMultiplier / CoreInUse->moveSpeedModifier);
		slowedTimer = slowedLength;
		break;
	case Electro:
		UGameplayStatics::GetAllActorsOfClass(Player->GetWorld(), ABaseEnemy::StaticClass(), FoundActors);
		
		for (AActor* actor : FoundActors)
		{
			ABaseEnemy* enemy = Cast<ABaseEnemy>(actor);
			// get location of enemy and AfflictedEnemy and check distance
			FVector tempLocation = enemy->GetActorLocation();
			float dist = FVector::Dist(AfflictedEnemyLocation, tempLocation);
			if (dist <= chainDistance * CoreInUse->chainRangeModifier && dist > 0 && enemy->Health > 0)
			{
				//enemy->Health -= chainDamage * CoreInUse->chainDamageModifier;
				enemy->Health -= Player->currentGun->damage * 1.0f * CoreInUse->chainDamageModifier;
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(Player->GetWorld(), Player->ARElectroParticle, AfflictedEnemyLocation, UKismetMathLibrary::FindLookAtRotation(AfflictedEnemyLocation, tempLocation), FVector(1,1,1), true, true);
			}
		}
		//Player->Afflictions.Remove(this);
		isDead = true;
		break;
	default: break;
	}
}

void StatusEffect::Freeze(ABaseEnemy* enemy, bool guaranteed)
{
	if (enemy == AfflictedEnemy)
	{
		int number = FMath::RandRange(0, 100);
		if (number <= CoreInUse->ChanceToAffect || guaranteed)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Freeze\n")));
			AfflictedEnemy->MoveSpeed() = 0;
		}
	}
}

bool StatusEffect::Update(float DeltaTime)
{
	if (dotTickTimer > 0)
	{
		dotTickTimer -= DeltaTime;
		if (dotTickTimer <= 0)
		{
			dotTickTimer = 0;
		}
	}

	if (dotTimer > 0)
	{
		dotTimer -= DeltaTime;
		if (dotTimer <= 0)
		{
			isDead = true;
		}
	}

	if (slowedTimer > 0)
	{
		slowedTimer -= DeltaTime;
		if (slowedTimer <= 0)
		{
			AfflictedEnemy->MoveSpeed() = baseEnemyMoveSpeed;
			isDead = true;
		}
	}

	switch (CoreInUse->CurrentElement)
	{
	case Pyro:
		if (CoreInUse->PerkMap[0] == 1 && dotTickTimer == 0 && AfflictedEnemy->Health > 0)
		{
			AfflictedEnemy->Health -= dotDamage * CoreInUse->dotDamageModifier;
			dotTickTimer = dotTickDelay / CoreInUse->dotTickModifier;
		}
		if (AfflictedEnemy->Health <= 0 && CoreInUse->PerkMap[5] == 1 && !hasFirePool)
		{
			FVector enemy_location =  AfflictedEnemy->GetActorLocation();
			CoreInUse->SpawnFirePool(enemy_location);
			//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Spawn Fire Pool\n")));
			hasFirePool = true;
		}
		break;

	default: break;
	}

	if (isDead)
	{
		return true;
	}
	return false;
		
}







