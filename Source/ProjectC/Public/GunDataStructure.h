// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Animation/AnimBlueprint.h"
#include "Engine/DataTable.h"

#include "GunDataStructure.generated.h"

USTRUCT(BlueprintType)
struct FGunDataStructure : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dmg;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float range;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool shotgun;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int projectiles;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool automatic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ammo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float reloadTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxRecoil;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minRecoil;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float recoilDeminishRate;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* gunMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> handsAnimBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> gunAnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> fireAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> reloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> swapWeaponAnim;
};
