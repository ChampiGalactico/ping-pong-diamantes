// Fill out your copyright notice in the Description page of Project Settings.
//
// Overview (theory):
// - GameInstance: engine object that persists across level transitions for a running game.
// - Use GameInstance to store global game settings or state that should survive level loads.
// - UPongGameInstance stores whether the game is in single-player mode or local coop. Accessible from GameMode and other systems.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PongGameInstance.generated.h"

/**
 * Global runtime data for the Pong game.
 */
UCLASS()
class PINGPONGDIAMANTES_API UPongGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// When true, spawn AI for the second paddle; when false, expect local coop input for player 2.
	UPROPERTY(BlueprintReadWrite, Category = "Pong")
	bool bIsSinglePlayerMode = true;
};
