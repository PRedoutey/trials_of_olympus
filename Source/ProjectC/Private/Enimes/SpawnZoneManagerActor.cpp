// Fill out your copyright notice in the Description page of Project Settings.


#include "Enimes/SpawnZoneManagerActor.h"
#include "NavigationSystem.h"
#include "PhysicsSettingsEnums.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnZoneManagerActor::ASpawnZoneManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnZoneManagerActor::BeginPlay()
{
	Super::BeginPlay();
	LoadArray();
	SpawnDelay = 10.0f;
	SpawnTimer = SpawnDelay;
	MinSpawnDelay = 5.0f;
	SpawnDelayDecrement = 0.3f;
	PlayerSafeDist = 500;
	SpawnBatchSize = 3;
	MaxEnemies = 50;
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AProjectCCharacter::StaticClass());
	Player = Cast<AProjectCCharacter>(FoundActor);
}

// Called every frame
void ASpawnZoneManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float ClosestDistance = 999999999999;
	int ClosestIndex = 0;
	for(int i = 0; i < SpawnZoneList.Num(); i++)
	{
		float dist = FVector::Dist(Player->GetActorLocation(), SpawnZoneList[i]->Pos);
		if (dist < ClosestDistance)
		{
			ClosestDistance = dist;
			ClosestIndex = i;
		}
	}
	CurrentZone = SpawnZoneList[ClosestIndex];
	if (SpawnTimer > 0)
	{
		SpawnTimer -= DeltaTime;
		if (SpawnTimer <= 0)
		{
			SpawnTimer = 0;
		}
	}
	if (SpawnTimer <= 0)
	{
		// spawn enemies

		// dont spawn past max enemy count
		TArray<AActor*> FoundActors;
		TArray<ABaseEnemy*> AliveEnemies;
		UGameplayStatics::GetAllActorsOfClass(Player->GetLevel(), ABaseEnemy::StaticClass(), FoundActors);
		for (AActor* actor : FoundActors)
		{
			ABaseEnemy* enemy = Cast<ABaseEnemy>(actor);
			if (enemy->Health > 0)
			{
				AliveEnemies.Add(enemy);
			}
		}
		int batch = SpawnBatchSize;
		if (AliveEnemies.Num() + SpawnBatchSize > MaxEnemies)
		{
			batch = MaxEnemies - AliveEnemies.Num();
		}
		
		
		FNavLocation ResultLocation;
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Spawn\n")));
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("%d\n"), batch));
		for (int i = 0; i < batch; i++)
		{
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Player->GetLevel());
			do
			{
				NavSystem->GetRandomReachablePointInRadius(CurrentZone->Pos, CurrentZone->Radius, ResultLocation);
			}
			while (FVector::Dist(Player->GetActorLocation(), FVector(ResultLocation)) < PlayerSafeDist);
			//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Spawn\n")));
			//SpawnActor<ACyclops>(Player->GetWorld(),ResultLocation, FRotator(0,0,0));
			//FActorSpawnParameters SpawnInfo;
			//Player->GetWorld()->SpawnActor<ACyclops>(ResultLocation, FRotator(0,0,0), SpawnInfo);
			// MyCyclopsSpawner->spawnCyclops(location);
			ResultLocation.Location.Z = 200;
			int number = FMath::RandRange(0, 100);
			if (number < 15)
			{
				SpawnCyclops(ResultLocation);
			}
			else
			{
				SpawnSoldier(ResultLocation);
			}
		}
		
		SpawnTimer = SpawnDelay;
		if (SpawnDelay > MinSpawnDelay)
		{
			SpawnDelay -= SpawnDelayDecrement;
		}
		
	}

}

void ASpawnZoneManagerActor::LoadArray()
{
	/*TArray<FString> OutStrings;
	FFileHelper::LoadANSITextFileToStrings(Fname, NULL, OutStrings);
	for(int i=0; i<= OutStrings.Max();i++)
	{
		FString X;
		FString Y;
		FString R;
		OutStrings[i].Split(",",&X,&Y);
		Y.Split(",",&Y,&R);
		SpawnZone* newZone = createSpawner(FVector(FCString::Atof(*X),FCString::Atof(*Y),1), FCString::Atof(*R));
		zoneList.Add(newZone);
	}*/

    //Close Right
	SpawnZone* zone1 = createSpawner(FVector(-3172.406494,-11731.741211, 1), 2419.0138590255);
	SpawnZone* zone2 = createSpawner(FVector(-1577.3573,-8076.984375, 1), 1815.8082249802);
	SpawnZone* zone3 = createSpawner(FVector(-5959.370605,-9014.389648, 1), 2536.160588107);
	SpawnZone* zone4 = createSpawner(FVector(-7031.597656,-5973.990234, 1), 2199.1106617578);
	SpawnZone* zone5 = createSpawner(FVector(-6545.463379,-2375.421387, 1), 2542.0549974981);
	SpawnZone* zone6 = createSpawner(FVector(-2862.897461,-2765.848145, 1), 2955.2401198977);
	SpawnZone* zone7 = createSpawner(FVector(-1924.324585,-4746.122559, 1), 1691.3585904875);
	SpawnZone* zone8 = createSpawner(FVector(-4191.289062,-5860.79248, 1), 2119.8591550088);

	//Close Left
	SpawnZone* zone9 = createSpawner(FVector(3099.996338,-2672.842285, 1), 2593.2963009473);
	SpawnZone* zone10 = createSpawner(FVector(6589.398438,-2449.604004, 1), 2489.4026945186);
	SpawnZone* zone11 = createSpawner(FVector(3099.998779,-6945.000000, 1), 2549.5147842337);
	SpawnZone* zone12 = createSpawner(FVector(7376.032715,-6892.594238, 1), 2811.8900732875);
	SpawnZone* zone13 = createSpawner(FVector(6100.000000,-9445.000000, 1), 1802.7807839634);
	SpawnZone* zone14 = createSpawner(FVector(2599.999023,-12045.000000, 1), 2061.5528128088);

	//Far Left (Noah's Far Left I've always said that) 
	SpawnZone* zone15 = createSpawner(FVector(2099.959717,8054.971680, 1), 2236.1113229052);
	SpawnZone* zone16 = createSpawner(FVector(5100.000000,8055.000000, 1), 2061.5513913082);
	SpawnZone* zone17 = createSpawner(FVector(1600.000488,4054.976318, 1), 2121.319826044);
	SpawnZone* zone18 = createSpawner(FVector(5500.998047,5355.000977, 1), 2549.511863476);
	SpawnZone* zone19 = createSpawner(FVector(6308.243652,2054.990112, 1), 1820.4578638062);
	SpawnZone* zone20 = createSpawner(FVector(3599.992432,1554.994873, 1), 2000.0036620089);
	
	//Far Right(as i am)
	SpawnZone* zone21 = createSpawner(FVector(-3040.0, 3040.0, 1),2593.2963009473);
	SpawnZone* zone22 = createSpawner(FVector(-4340.0, 4180.0, 1),2593.2963009473);
	SpawnZone* zone23 = createSpawner(FVector(-4180.0, 3040.0, 1),2593.2963009473);
	SpawnZone* zone24 = createSpawner(FVector(-2910.0, 8610.0, 1),2593.2963009473);
	SpawnZone* zone25 = createSpawner(FVector(-3990.0, 1780.0, 1),2593.2963009473);
	SpawnZone* zone26 = createSpawner(FVector(-6840.0, 1780.0, 1),2593.2963009473);
	
	SpawnZoneList.Add(zone1);
	SpawnZoneList.Add(zone2);
	SpawnZoneList.Add(zone3);
	SpawnZoneList.Add(zone4);
	SpawnZoneList.Add(zone5);
	SpawnZoneList.Add(zone6);
	SpawnZoneList.Add(zone7);
	SpawnZoneList.Add(zone8);
	SpawnZoneList.Add(zone9);
	SpawnZoneList.Add(zone10);
	SpawnZoneList.Add(zone11);
	SpawnZoneList.Add(zone12);
	SpawnZoneList.Add(zone13);
	SpawnZoneList.Add(zone14);
	SpawnZoneList.Add(zone15);
	SpawnZoneList.Add(zone16);
	SpawnZoneList.Add(zone17);
	SpawnZoneList.Add(zone18);
	SpawnZoneList.Add(zone19);
	SpawnZoneList.Add(zone20);
	SpawnZoneList.Add(zone21);
	SpawnZoneList.Add(zone22);
	SpawnZoneList.Add(zone23);
	SpawnZoneList.Add(zone24);
	SpawnZoneList.Add(zone25);
	SpawnZoneList.Add(zone26);
}

