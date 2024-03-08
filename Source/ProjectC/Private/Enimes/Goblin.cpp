// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enimes/Goblin.h"
#include "DrawDebugHelpers.h"
#include "../Public/Enimes/GoblinController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"

AGoblin::AGoblin(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	oldHealth = Health;
	const FString SkeletonPath = "/Game/Assets/SKM/SKM_Enemy_Goblin_Chrzd_B_DP";
	GetMesh()->SetSkeletalMesh(Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *SkeletonPath)));
	/*GetMesh()->SetRelativeLocation(FVector(0.0,0.0,-80.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0,-105.0,0.0));
	GetMesh()->SetRelativeScale3D(FVector(1.0,1.0,1.0));
	
	GetCapsuleComponent()->SetCapsuleSize(30.0, 105.0);*/

	auto const DeathAnimAsset = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("AnimMontage'/Game/FirstPerson/Animations/Goblin/GoblinDieMontage.GoblinDieMontage'"));
	if (DeathAnimAsset.Object != nullptr)
	{
		DeathAnim = DeathAnimAsset.Object;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AGoblinController::StaticClass();
}

void AGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoblin::Die()
{
	if (Cast<ABaseEnemyController>(GetController()))
	{
		Cast<ABaseEnemyController>(GetController())->BehaviorTreeComponent->StopTree();
	}
	PlayAnimMontage(DeathAnim, 1.0f, FName(TEXT("Start")));
	Super::Die();
}