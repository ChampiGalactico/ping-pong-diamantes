// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameMode.h"
#include "../Paddle/PongPaddle.h"
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
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	UPongGameInstance* PongGameInstance = Cast<UPongGameInstance>(GetGameInstance());
	bool bIsSinglePlayer = PongGameInstance ? PongGameInstance->bIsSinglePlayerMode : true;

	// Spawn both Paddles at their respective locations
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APongPaddle* PaddleP1 = World->SpawnActor<APongPaddle>(PaddleClass, Paddle1SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	APongPaddle* PaddleP2 = World->SpawnActor<APongPaddle>(PaddleClass, Paddle2SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	if (PaddleP1)
	{
		PaddleP1->InputMappingContext = Player1MappingContext;
	}
	if (PaddleP2)
	{
		PaddleP2->InputMappingContext = Player2MappingContext;
	}

	/*
	* We assign InputMappingContext after spawning but before Possess. Remember: Possess is what triggers SetupPlayerInputComponent internally, 
	and BeginPlay on the paddle (which is where we call AddMappingContext) runs at spawn time too. So we need InputMappingContext to already have 
	the correct value before those events fire — otherwise BeginPlay's check if (InputMappingContext) would find it still nullptr 
	(since it defaults to unset), and skip activating it entirely.
	*/

	APlayerController* PlayerController1 = UGameplayStatics::GetPlayerController(World, 0);

	if (PlayerController1 && PaddleP1)
	{
		PlayerController1->Possess(PaddleP1);
		PaddleP1->ActivateInput();
	}

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
		// TODO: Spawn AI Paddle for Player 2
	}
	else
	{
		// Coop local: mismo PlayerController escucha ambos Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController1->GetLocalPlayer()))
		{
			if (Player2MappingContext)
			{
				Subsystem->AddMappingContext(Player2MappingContext, 0);
			}
		}

		if (UEnhancedInputComponent* EnhancedIC = Cast<UEnhancedInputComponent>(PlayerController1->InputComponent))
		{
			if (MoveAction_P2 && PaddleP2)
			{
				EnhancedIC->BindAction(MoveAction_P2, ETriggerEvent::Triggered, PaddleP2, &APongPaddle::Move);
				EnhancedIC->BindAction(MoveAction_P2, ETriggerEvent::Completed, PaddleP2, &APongPaddle::Move);
			}
		}
	}
}


