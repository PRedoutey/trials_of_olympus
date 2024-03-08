// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/FirePoolActor.h"
#include "../Public/Enimes/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/Character/ProjectCCharacter.h"

// Sets default values
AFirePoolActor::AFirePoolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	lifetime = 3;

}

// Called when the game starts or when spawned
void AFirePoolActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirePoolActor::Tick(float DeltaTime)
{
	AProjectCCharacter* player = Cast<AProjectCCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AProjectCCharacter::StaticClass()));
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(player->GetWorld(), ABaseEnemy::StaticClass(), FoundActors);
	for (AActor* actor : FoundActors)
	{
		ABaseEnemy* enemy = Cast<ABaseEnemy>(actor);
		FVector enemyLocation = enemy->GetActorLocation();
		float dist = FVector::Dist(GetActorLocation(), enemyLocation);
		if (dist <= 300 && !enemy->IsAffected && enemy->Health > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Burn\n")));
			ParentCore->CreateAffliction(player, enemy);
		}
	}
	lifetime -= DeltaTime;
	if (lifetime <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Destroy\n")));
		Destroy();
	}
}


void AFirePoolActor::SetCore(AGunCore* core)
{
	ParentCore = core;
}

