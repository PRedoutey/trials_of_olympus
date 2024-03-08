// Fill out your copyright notice in the Description page of Project Settings.
#include "../Public/Enimes/MeleeGolem.h"

#include "DrawDebugHelpers.h"
#include "../Public/Enimes/MeleeGolemController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMeleeGolem::AMeleeGolem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	const FString SkeletonPath = "/Game/FirstPerson/Enemies/Mesh/MeleeGolem/EnemyMelee_Char_V1_DP_SP23.EnemyMelee_Char_V1_DP_SP23";
	GetMesh()->SetSkeletalMesh(Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *SkeletonPath)));
	GetMesh()->SetRelativeLocation(FVector(0.0,0.0,-80.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0,-90.0,0.0));
	GetMesh()->SetRelativeScale3D(FVector(.5,.5,.5));
	
	//GetCapsuleComponent()->SetCapsuleSize(30.0, 125.0);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//AIControllerClass = TSubclassOf<ARangedGolemController>();
	AIControllerClass = AMeleeGolemController::StaticClass();
}