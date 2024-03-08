// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Generator.h"

#include "NavigationSystem.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxCount = 5;
	CurCount = 0;
	isActive = false;
	MaxDistance = 2000;

}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGenerator::AddCount()
{
	CurCount++;
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("%d\n"), CurCount));
	if (CurCount >= MaxCount && isActive == true)
	{
		isActive = false;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetLevel(), ADeathDust, MyGenerator->GetTargetLocation());
		
		FNavLocation ResultLocation;
        UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomReachablePointInRadius(GetActorLocation(), 500, ResultLocation);
        SpawnGoblin(ResultLocation);
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, "Generator Deactivated\n");
	}
}

bool AGenerator::GetIsActive()
{
	return isActive;
}

void AGenerator::SetIsActive(bool Active)
{
	isActive = Active;
}


