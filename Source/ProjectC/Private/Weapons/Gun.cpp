// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Weapons/Gun.h"

#include "../Public/Enimes/RangedGolem.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void AGun::SetStats(FGunDataStructure inputStruct)
{
	type = inputStruct.type;
	damage = inputStruct.dmg;
	speedModifier = inputStruct.speed;
	rpm = inputStruct.fireRate;
	dmgRange = inputStruct.range;
	isShotgun = inputStruct.shotgun;
	numProjectiles = inputStruct.projectiles;
	isAutomatic = inputStruct.automatic;
	weaponMesh = inputStruct.gunMesh;
	fireDelay = 1 / (rpm / 60);
	MaxAmmo = inputStruct.ammo;
	curAmmo = inputStruct.ammo;
	ReloadSpeed = inputStruct.reloadTime;
	StartingRecoil = -inputStruct.maxRecoil;
	MinRecoilPerShot = -inputStruct.minRecoil;
	RecoilDeminishingRate = inputStruct.recoilDeminishRate;
	animBP = inputStruct.handsAnimBP;
	fireAnimation = inputStruct.fireAnim;
	reloadAnimation = inputStruct.reloadAnim;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	this->AddToRoot();
	Super::BeginPlay();
	//this->AddToRoot();
}

void AGun::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UObjectBaseUtility::RemoveFromRoot();
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fireCooldown -= DeltaTime;
	if (fireCooldown < 0)
	{
		fireCooldown = 0;
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

