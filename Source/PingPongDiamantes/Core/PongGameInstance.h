// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PongGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONGDIAMANTES_API UPongGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Pong")
	bool bIsSinglePlayerMode = false;
};
