// Fill out your copyright notice in the Description page of Project Settings.
//
// Brief theory and notes:
// - BeginPlay performs initial runtime setup: spawn paddles, set materials, possess player 1, configure input contexts and set camera.
// - GameMode is the correct place to spawn game actors that represent players and to decide singleplayer vs local coop behavior.

#include "PongGameMode.h"
#include "../Paddle/PongPaddle.h"
#include "../Ball/PongBall.h"
#include "AI/PongAIController.h"
#include "PongGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"

void APongGameMode::BeginPlay() 
{
	Super::BeginPlay();

	if (!PaddleClass)
	{
		// No paddle class provided: nothing to spawn.
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	// Query game instance for mode (singleplayer/co-op).
	UPongGameInstance* PongGameInstance = Cast<UPongGameInstance>(GetGameInstance());
	bool bIsSinglePlayer = PongGameInstance ? PongGameInstance->bIsSinglePlayerMode : true;

	// Spawn both Paddles at their respective locations
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APongPaddle* PaddleP1 = World->SpawnActor<APongPaddle>(PaddleClass, Paddle1SpawnLocation, FRotator(0.0f, 90.0f, 0.0f), SpawnParams);
	APongPaddle* PaddleP2 = World->SpawnActor<APongPaddle>(PaddleClass, Paddle2SpawnLocation, FRotator(0.0f, -90.0f, 0.0f), SpawnParams);

	// Assign input mapping contexts and materials if available.
	if (PaddleP1)
	{
		PaddleP1->InputMappingContext = Player1MappingContext;
		if (PaddleMaterialP1)
		{
			PaddleP1->PaddleMesh->SetMaterial(0, PaddleMaterialP1);
		}
	}
	if (PaddleP2)
	{
		PaddleP2->InputMappingContext = Player2MappingContext;
		if (PaddleMaterialP2)
		{
			PaddleP2->PaddleMesh->SetMaterial(0, PaddleMaterialP2);
		}
	}

	// Get player controller 0 (local player)
	APlayerController* PlayerController1 = UGameplayStatics::GetPlayerController(World, 0);

	if (PlayerController1 && PaddleP1)
	{
		// Possess the first paddle with the player's controller so they can move it.
		PlayerController1->Possess(PaddleP1);
		PaddleP1->ActivateInput();
	}

	// Set input mode and cursor visibility
	PlayerController1->SetInputMode(FInputModeGameOnly());
	PlayerController1->bShowMouseCursor = false;

	// Find camera actor by tag and set view target to it (keeps camera logic in the level).
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsWithTag(World, FName("MainGameCamera"), FoundCameras);

	if (FoundCameras.Num() > 0)
	{
		ACameraActor* GameCamera = Cast<ACameraActor>(FoundCameras[0]);

		if (GameCamera)
		{
			PlayerController1->SetViewTargetWithBlend(GameCamera, 0.0f);
		}
	}

	if (bIsSinglePlayer)
	{
		if (PaddleP2)
		{
			APongAIController* AIController = World->SpawnActor<APongAIController>();
			if (AIController)
			{
				AIController->Possess(PaddleP2);
				AIController->InitializeAI();
				CachedAIController = AIController;
				UE_LOG(LogTemp, Warning, TEXT("AIController spawned and possessing Paddle2"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn AIController"));
			}
		}
	}
	else
	{
		// Coop local: same PlayerController listens to both paddles, but we add a second mapping context
		// so Player 2's controls can be mapped separately and then bind MoveAction_P2 to the second paddle.

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController1->GetLocalPlayer()))
		{
			if (Player2MappingContext)
			{
				Subsystem->AddMappingContext(Player2MappingContext, 0);
			}
		}

		// Bind MoveAction_P2 to PaddleP2's Move() method on the PlayerController's input component.
		if (UEnhancedInputComponent* EnhancedIC = Cast<UEnhancedInputComponent>(PlayerController1->InputComponent))
		{
			if (MoveAction_P2 && PaddleP2)
			{
				EnhancedIC->BindAction(MoveAction_P2, ETriggerEvent::Triggered, PaddleP2, &APongPaddle::Move);
				EnhancedIC->BindAction(MoveAction_P2, ETriggerEvent::Completed, PaddleP2, &APongPaddle::Move);
			}
		}
	}

	SpawnBall();
}

void APongGameMode::SpawnBall()
{
	if (!BallClass)
	{
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APongBall* NewBall = World->SpawnActor<APongBall>(BallClass, BallSpawnLocation, FRotator::ZeroRotator);

	if (CachedAIController && NewBall)
	{
		CachedAIController->SetTargetBall(NewBall);
	}
}

void APongGameMode::OnGoalScored(int32 ScoringPlayerIndex, APongBall* ScoredBall)
{
	if (ScoringPlayerIndex == 1)
	{
		Player1Score++;
	}
	else
	{
		Player2Score++;
	}

	UE_LOG(LogTemp, Warning, TEXT("GOAL! Player1: %d - Player2: %d"), Player1Score, Player2Score);

	if (ScoredBall)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying ball: %s"), *ScoredBall->GetName());
		ScoredBall->Destroy();
	}

	SpawnBall();
}
