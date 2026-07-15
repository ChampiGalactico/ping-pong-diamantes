// Fill out your copyright notice in the Description page of Project Settings.


#include "PongAIController.h"
#include "../../Paddle/PongPaddle.h"
#include "../../Ball/PongBall.h"
#include "Kismet/GameplayStatics.h"

void APongAIController::SetTargetBall(APongBall* NewBall)
{
	TargetBall = NewBall;
}


void APongAIController::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
}

void APongAIController::InitializeAI()
{
	ControlledPaddle = Cast<APongPaddle>(GetPawn());
	UE_LOG(LogTemp, Warning, TEXT("AIController BeginPlay - ControlledPaddle: %s"), ControlledPaddle ? TEXT("Valid") : TEXT("NULL"))
}

void APongAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ControlledPaddle)
	{
		UE_LOG(LogTemp, Error, TEXT("No ControlledPaddle in Tick"));
		return;
	}

	if (!TargetBall)
	{
		return;
		
	}

	FVector CurrentLocation = ControlledPaddle->GetActorLocation();
	FVector TargetLocation= CurrentLocation;
	TargetLocation.Y = TargetBall->GetActorLocation().Y;

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, AISpeed);
	ControlledPaddle->SetActorLocation(NewLocation, true);
}