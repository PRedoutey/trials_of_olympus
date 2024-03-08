// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enimes/BaseEnemyController.h"


// Sets default values
ABaseEnemyController::ABaseEnemyController(const class FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	
	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("EnemyAIBlackboard"));
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("EnemyAIBehaviorTree"));
}

// Called when the game starts or when spawned
void ABaseEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTree, EBTExecutionMode::Looped);
	}
}

void ABaseEnemyController::SetBehaviorTree(const FString BTPath)
{
	if (!BehaviorTree)
		BehaviorTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, *BTPath));
}

// Called every frame
void ABaseEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

