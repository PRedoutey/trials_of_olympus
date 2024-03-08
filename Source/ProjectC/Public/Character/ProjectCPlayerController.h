// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTC_API AProjectCPlayerController : public APlayerController
{
	GENERATED_BODY()


	AProjectCPlayerController();

	virtual void SetupInputComponent() override;

	void OpenMenu();
	
};
