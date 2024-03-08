// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enimes/RangedGolem.h"
#include "../Public/Enimes/RangedGolemController.h"

// Sets default values
ARangedGolem::ARangedGolem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	const FString SkeletonPath = "/Game/FirstPerson/Enemies/Mesh/RangedGolem/EnemyRanged_Characterized_V1_DP_SP23";
	GetMesh()->SetSkeletalMesh(Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *SkeletonPath)));
	GetMesh()->SetRelativeLocation(FVector(0.0,0.0,-80.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0,-90.0,0.0));
	GetMesh()->SetRelativeScale3D(FVector(0.5,0.5,0.5));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//AIControllerClass = TSubclassOf<ARangedGolemController>();
	AIControllerClass = ARangedGolemController::StaticClass();
}

