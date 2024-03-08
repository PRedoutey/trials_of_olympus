// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunDataStructure.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class PROJECTC_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	
	// convert fireDelay to rpm
	// (1 / fireDelay) * 60 = rpm
	
	// convert rpm to fireDelay
	// 1 / (rpm / 60) = fireDelay
	// AR -> 450-600rpm
	// SMG -> 750-950rpm
	// Pistol -> Uncapped semi
	// Shotgun -> rpm
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float despawnTimer = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isEquipped = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName type;
	float rpm;
	float damage;
	float speedModifier;
	float fireDelay;
	float fireCooldown;	// if zero, you can fire
	float dmgRange;
	bool isShotgun;
	int numProjectiles;
	bool isAutomatic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int curAmmo;
	float ReloadSpeed;
	float StartingRecoil;
	float MinRecoilPerShot;
	float RecoilDeminishingRate;
	USkeletalMesh* weaponMesh;
	TSubclassOf<UAnimInstance> animBP;
	TArray<UAnimMontage*> fireAnimation;
	TArray<UAnimMontage*> reloadAnimation;


	
	UFUNCTION(BlueprintCallable)
	void SetStats(FGunDataStructure inputStruct);

	

protected:
	// Called when the game starts or when spawned
	void BeginPlay();

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

};
