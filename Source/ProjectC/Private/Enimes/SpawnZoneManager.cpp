// Fill out your copyright notice in the Description page of Project Settings.


#include "Enimes/SpawnZoneManager.h"

#include "Enimes/Cyclops.h"
#include "Enimes/SpawnZone.h"
//#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem.h"
//#include "ToolContextInterfaces.h"

template< class T >
T* SpawnActor
(
	UWorld* world,
	FVector const& Location,
	FRotator const& Rotation,
	AActor* Owner=NULL,
	APawn* Instigator=NULL,
	bool bNoCollisionFail=false
)
{
	//return (T*)(world->SpawnActor(T::StaticClass(), NAME_None, &Location, &Rotation, NULL, bNoCollisionFail, false, Owner, Instigator));
	return (T*)(world->SpawnActor(T::StaticClass(), &Location, &Rotation));
}


SpawnZoneManager::SpawnZoneManager()
{
	//TCHAR fname[21] = TEXT("/Game/SpawnZones.txt");
	LoadArray();
	SpawnDelay = 10.0f;
	SpawnTimer = 20.0f;
	PlayerSafeDist = 0;
}

SpawnZoneManager::~SpawnZoneManager()
{
}

void SpawnZoneManager::LoadArray()
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
	SpawnZone* zone1 = createSpawner(FVector(-3172.406494,-11731.741211, 1), 2419.0138590255);
	SpawnZone* zone2 = createSpawner(FVector(-1577.3573,-8076.984375, 1), 1815.8082249802);
	SpawnZone* zone3 = createSpawner(FVector(-5959.370605,-9014.389648, 1), 2536.160588107);
	SpawnZone* zone4 = createSpawner(FVector(-7454.225098,-5826.695312, 1), 2199.1106617578);
	SpawnZone* zone5 = createSpawner(FVector(-6545.463379,-2375.421387, 1), 2542.0549974981);
	SpawnZone* zone6 = createSpawner(FVector(-2862.897461,-2765.848145, 1), 2955.2401198977);
	SpawnZone* zone7 = createSpawner(FVector(-1924.324585,-4746.122559, 1), 1691.3585904875);
	SpawnZone* zone8 = createSpawner(FVector(-4191.289062,-5860.79248, 1), 2119.8591550088);
	SpawnZoneList.Add(zone1);
	SpawnZoneList.Add(zone2);
	SpawnZoneList.Add(zone3);
	SpawnZoneList.Add(zone4);
	SpawnZoneList.Add(zone5);
	SpawnZoneList.Add(zone6);
	SpawnZoneList.Add(zone7);
	SpawnZoneList.Add(zone8);
}

void SpawnZoneManager::Update(float dt)
{
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
		SpawnTimer -= dt;
		if (SpawnTimer <= 0)
		{
			SpawnTimer = 0;
		}
	}
	if (SpawnTimer <= 0)
	{
		FNavLocation ResultLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Player->GetWorld());
		do
		{
			NavSystem->GetRandomReachablePointInRadius(CurrentZone->Pos, CurrentZone->Radius, ResultLocation);
		}
		while (FVector::Dist(Player->GetActorLocation(), FVector(ResultLocation)) < PlayerSafeDist);
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString::Printf(TEXT("Spawn\n")));
		//SpawnActor<ACyclops>(Player->GetWorld(),ResultLocation, FRotator(0,0,0));
		//FActorSpawnParameters SpawnInfo;
		//Player->GetWorld()->SpawnActor<ACyclops>(ResultLocation, FRotator(0,0,0), SpawnInfo);
		// MyCyclopsSpawner->spawnCyclops(location);
		SpawnTimer = SpawnDelay;
	}
	
	
}

void SpawnZoneManager::Set_Player(AProjectCCharacter* player)
{
	Player = player;
}


/*void readSceneFile(std::string filename, World& world)
{
	std::ifstream file;
	file.open(filename);
	std::string new_line;
    
	while (!file.eof())
	{
		std::string type;
		file >> type;
		if (type == "sphere")
		{
			float x, y, z, rad, r, g, b;
			file >> x >> y >> z >> rad >> r >> g >> b;
			world.spheres.push_back(Sphere(ssu::vec3(x, y, z), rad, ssu::vec3(r, g, b)));
		}
		else if (type == "mesh")
		{
			std::string fname;
			float x, y, z, r, g, b;
			file >> fname >> x >> y >> z >> r >> g >> b;
			readSTL(fname, world, ssu::vec3(r, g, b), ssu::vec3(x, y, z));
		}
	}
}*/

//SpawnZone::createSpawner(FVector pos, float radius)