// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PongGameMode.generated.h"

class APongPaddle;
class UInputMappingContext;
class UInputAction;
class ACameraActor;

/**
 * 
 */
UCLASS()
class PINGPONGDIAMANTES_API APongGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Pong")
	TSubclassOf<APongPaddle> PaddleClass;

	UPROPERTY(EditAnywhere, Category = "Pong")
	FVector Paddle1SpawnLocation = FVector(600.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Pong")
	FVector Paddle2SpawnLocation = FVector(-600.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category="Pong")
	UInputMappingContext* Player1MappingContext;

	UPROPERTY(EditAnywhere, Category = "Pong")
	UInputMappingContext* Player2MappingContext;

	UPROPERTY(EditAnywhere, Category = "Pong")
	UInputAction* MoveAction_P2;
	
};
