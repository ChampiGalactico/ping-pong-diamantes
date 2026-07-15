// Fill out your copyright notice in the Description page of Project Settings.
//
// Brief theory and notes:
// - This Pawn uses Enhanced Input: InputMappingContext and InputAction are part of UE's Enhanced Input system.
// - `SetupPlayerInputComponent` is called by the engine when a PlayerController possesses this Pawn.
// - `ActivateInput` registers the Pawn's mapping context with the local player's Enhanced Input subsystem so the actions become active.
// - Movement is applied in Tick to keep movement frame-rate independent.

#include "PongPaddle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

// Sets default values
APongPaddle::APongPaddle()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create and attach the static mesh used as the paddle body.
	PaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleMesh"));
	RootComponent = PaddleMesh;
}

// Called when the game starts or when spawned
void APongPaddle::BeginPlay()
{
	Super::BeginPlay();
}

// SetupPlayerInputComponent:
// - Called automatically when a PlayerController possesses this Pawn.
// - We cast to UEnhancedInputComponent to access BindAction (Enhanced Input).
void APongPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction_P1)
		{
			// Bind both Triggered and Completed so we receive axis values while active and zero when completed.
			EnhancedInputComponent->BindAction(MoveAction_P1, ETriggerEvent::Triggered, this, &APongPaddle::Move);
			EnhancedInputComponent->BindAction(MoveAction_P1, ETriggerEvent::Completed, this, &APongPaddle::Move);
		}
	}
}

// Move:
// - Called by Enhanced Input with the axis value. We store the float and apply movement in Tick.
void APongPaddle::Move(const FInputActionValue& Value)
{
	CurrentMoveInput = Value.Get<float>();
}

// ActivateInput:
// - Adds this Pawn's InputMappingContext to the local player's Enhanced Input subsystem.
// - This should be called after possession so the local player's mappings become active for this Pawn.
void APongPaddle::ActivateInput() 
{
	UE_LOG(LogTemp, Warning, TEXT("ActivateInput called on %s"), *GetName());

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController valid"));
		// Get the Enhanced Input subsystem for the local player.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Subsystem valid"));
			if (InputMappingContext)
			{
				UE_LOG(LogTemp, Warning, TEXT("InputMappingContext valid, adding it"));
				// AddMappingContext registers the mapping context with a priority (0 here).
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("InputMappingContext is NULL"));
			}
		}
	}
}

// Called every frame
// - Applies movement to the actor location along the Y axis based on CurrentMoveInput.
void APongPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentMoveInput != 0.0f)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Y += CurrentMoveInput * MoveSpeed * DeltaTime;
		// Sweep to true to stop on collisions (if any).
		SetActorLocation(NewLocation, true);
	}

}

