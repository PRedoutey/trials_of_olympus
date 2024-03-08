// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enimes/SoldierController.h"


ASoldierController::ASoldierController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Change this path to the desired behavior tree. For reference, 'Game' is the Content folder
	SetBehaviorTree("/Game/FirstPerson/Enemies/SoldierBT");
}