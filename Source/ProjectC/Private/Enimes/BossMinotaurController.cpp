// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enimes/BossMinotaurController.h"


ABossMinotaurController::ABossMinotaurController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Change this path to the desired behavior tree. For reference, 'Game' is the Content folder
	SetBehaviorTree("/Game/FirstPerson/Enemies/BossMinotaurBT");
}