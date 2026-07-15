// Fill out your copyright notice in the Description page of Project Settings.
//
// Overview (theory):
// - GameMode: server-side class that defines game rules and high-level flow for a level.
// - APongGameMode is responsible for spawning player pawns (paddles), assigning input contexts, and configuring camera/input settings.
// - GameMode exists only on the server/editor; clients use GameState/PlayerState for replicated info.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PongGameMode.generated.h"

class APongPaddle;
class APongAIController;
class APongBall;
class UInputMappingContext;
class UInputAction;
class ACameraActor;

/**
 * Pong game rules and initial setup.
 */
UCLASS()
class PINGPONGDIAMANTES_API APongGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Called when the level starts; used here to spawn paddles and configure input.
	virtual void BeginPlay() override;

	// Class used to spawn paddle actors.
	UPROPERTY(EditAnywhere, Category = "Pong")
	TSubclassOf<APongPaddle> PaddleClass;

	// Default spawn positions for both paddles (editable in the editor).
	UPROPERTY(EditAnywhere, Category = "Pong")
	FVector Paddle1SpawnLocation = FVector(900.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Pong")
	FVector Paddle2SpawnLocation = FVector(-900.f, 0.f, 0.f);

	// Input mapping contexts for player 1 and player 2 (Enhanced Input).
	UPROPERTY(EditAnywhere, Category="Pong")
	UInputMappingContext* Player1MappingContext;

	UPROPERTY(EditAnywhere, Category = "Pong")
	UInputMappingContext* Player2MappingContext;

	// Move action for player 2 when binding on controller input component.
	UPROPERTY(EditAnywhere, Category = "Pong")
	UInputAction* MoveAction_P2;
	
	// Optional materials to differentiate paddles visually.
	UPROPERTY(EditAnywhere, Category = "Paddle")
	UMaterialInterface* PaddleMaterialP1;

	UPROPERTY(EditAnywhere, Category = "Paddle")
	UMaterialInterface* PaddleMaterialP2;

	UPROPERTY(EditAnywhere, Category = "Pong")
	TSubclassOf<APongBall> BallClass;

	UPROPERTY(EditAnywhere, Category = "Pong")
	FVector BallSpawnLocation = FVector(0.0f, 0.0f, 50.0f);

	UPROPERTY(BlueprintReadOnly, Category = "Pong")
	int32 Player1Score = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Pong")
	int32 Player2Score = 0;

	void OnGoalScored(int32 ScoringPlayerIndex, APongBall* ScoredBall);

private:
	void SpawnBall();
	APongAIController* CachedAIController;
};
