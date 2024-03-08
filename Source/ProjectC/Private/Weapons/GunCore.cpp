// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Weapons/GunCore.h"
#include "../Public/Character/ProjectCCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/List.h"

//#include "../../../../../Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.30.30705/include/list"

// Enum of all of the elements currently possible for cores
//enum Element { Fire, Ice, Lightning };

// Current element of the instance of a core
//Element CurrentElement;

// List of all perks that are active on a core
//TArray<int32> PerkList;

// CURRENTLY NOT IN USE; List of perks that are locked, once unlocking perks is available,
//these will move to the PerkList[] when unlocked
//int LockedPerkList[];



// Sets default values
AGunCore::AGunCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	int number = FMath::RandRange(0, 2);
    switch (number)
    {
    case 0:
    	CurrentElement = {Pyro};
    	break;
    case 1:
    	CurrentElement = {Cryo};
    	break;
    case 2:
    	CurrentElement = {Electro};
    	break;
    }

	// seed 3 perks
	int num;
	for (int i = 0; i < 3; i++)
	{
		do
		{
			num = FMath::RandRange(1,5);
		}
		while (AquirablePerks.Contains(num));
		AquirablePerks.Add(num);
	}
	AffectDelay = 2;
	AffectTimer = 0;
	ChanceToAffect = 30;
	damageModifier = 2.0f;
	damageIntakeModifier = 1.0f;

	dotDamageModifier = 1.0f;
	dotTickModifier = 1.0f;
	firedamageModifier = 1.0f;

	slowingModifier = 1.0f;

	chainRangeModifier = 1.0f;
	moveSpeedModifier = 1.0f;
	reloadSpeedModifer = 1.0f;
	chainDamageModifier = 1.0f;
	dashModifier = 1.0f;
	
	PerkMap.Add(0,1);
	PerkMap.Add(1,0);
	PerkMap.Add(2,0);
	PerkMap.Add(3,0);
	PerkMap.Add(4,0);
	PerkMap.Add(5,0);
	
}

void AGunCore::SetElement(Element element)
{
	CurrentElement = element;
	
	// tmp
	//OnAcquire(1);
	//OnAcquire(2);
	//OnAcquire(3);
	//OnAcquire(4);
	//OnAcquire(5);
}

int AGunCore::getType()
{
	return TEnumAsByte<Element>(AGunCore::CurrentElement);
}

void AGunCore::OnAcquire(int perk)
{
	switch (CurrentElement)
	{
	case Pyro:
		// pyro
		if (perk == 1)
		{
			PerkMap.Add(1,1);
			dotDamageModifier = 2.0f;
		}
		else if (perk == 2)
		{
			PerkMap.Add(2,1);
			ChanceToAffect = 20.0f;
		}
		else if (perk == 3)
		{
			PerkMap.Add(3,1);
			dotTickModifier = 2.0f;
		}
		else if (perk == 4)
		{
			PerkMap.Add(4,1);
			firedamageModifier = 1.25f;
		}
		else if (perk == 5)
		{
			PerkMap.Add(5,1);
		}
		break;
	case Cryo:
		// cryo
		if (perk == 1)
		{
			PerkMap.Add(1,1);
			slowingModifier = 2.0f;
		}
		else if (perk == 2)
		{
			PerkMap.Add(2,1);
			ChanceToAffect = 20.0f;
		}
		else if (perk == 3)
		{
			PerkMap.Add(3,1);
			damageIntakeModifier = 0.8;
		}
		else if (perk == 4)
		{
			PerkMap.Add(4,1);
		}
		else if (perk == 5)
		{
			PerkMap.Add(5,1);
		}
		break;
	case Electro:
		// electro
		if (perk == 1)
		{
			PerkMap.Add(1,1);
			chainRangeModifier = 2.0f;
		}
		else if (perk == 2)
		{
			PerkMap.Add(2,1);
			ChanceToAffect = 20.0f;
		}
		else if (perk == 3)
		{
			PerkMap.Add(3,1);
			moveSpeedModifier = 1.1f;
			reloadSpeedModifer = 1.25f;
		}
		else if (perk == 4)
		{
			PerkMap.Add(4,1);
			chainDamageModifier = 2.0f;
		}
		else if (perk == 5)
		{
			PerkMap.Add(5,1);
			dashModifier = 2.0f;
		}
		break;
	}
}

StatusEffect* AGunCore::CreateAffliction(AProjectCCharacter* player, ABaseEnemy* enemy)
{
	StatusEffect* new_affliction = new StatusEffect();
	new_affliction->Set_Core(this);
	new_affliction->Set_Enemy(enemy);
	new_affliction->Set_Player(player);
	new_affliction->OnApply();
	player->Afflictions.Add(new_affliction);
	enemy->IsAffected = true;
	AffectTimer = AffectDelay;
	return new_affliction;
}

/*void AGunCore::CreateFireAfflictionsBP(FVector location)
{

	AProjectCCharacter* player = Cast<AProjectCCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AProjectCCharacter::StaticClass()));
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(player->GetWorld(), ABaseEnemy::StaticClass(), FoundActors);
		
	for (AActor* actor : FoundActors)
	{
		ABaseEnemy* enemy = Cast<ABaseEnemy>(actor);
		// get location of enemy and AfflictedEnemy and check distance
		FVector enemyLocation = enemy->GetActorLocation();
		float dist = FVector::Dist(location, enemyLocation);
		if (dist <= 100 && !enemy->IsAffected)
		{
			CreateAffliction(player, enemy);
		}
	}
}*/

// Called when the game starts or when spawned
void AGunCore::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGunCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AffectTimer > 0)
	{
		AffectTimer -= DeltaTime;
		if (AffectTimer <= 0)
		{
			AffectTimer = 0;
		}
	}
	
	if (!isEquipped)
	{
		despawnTimer -= DeltaTime;
	}
	if (despawnTimer <= 0)
	{
		this->Destroy();
	}
}

AGunCore* CreateNewGunCore(Element element)
{
	AGunCore* newCore = NewObject<AGunCore>();
	newCore->SetElement(element);
	return newCore;
}

