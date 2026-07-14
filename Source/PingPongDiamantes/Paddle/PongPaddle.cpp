// Fill out your copyright notice in the Description page of Project Settings.


#include "PongPaddle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

// Sets default values
APongPaddle::APongPaddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleMesh"));
	RootComponent = PaddleMesh;

}

// Called when the game starts or when spawned
void APongPaddle::BeginPlay()
{
	Super::BeginPlay();
}

void APongPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
/* You will never call it yourself. Unreal calls it automatically, at the exact moment a PlayerController possesses this Pawn. 
This is the moment when you can bind your input actions to the functions that will handle them. */
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	/* The parameter PlayerInputComponent comes in as the generic base type UInputComponent*. 
	But Enhanced Input's specific binding function (BindAction) only exists on the more specific subclass UEnhancedInputComponent.*/
	{
		if (MoveAction_P1)
		{
			EnhancedInputComponent->BindAction(MoveAction_P1, ETriggerEvent::Triggered, this, &APongPaddle::Move);
			EnhancedInputComponent->BindAction(MoveAction_P1, ETriggerEvent::Completed, this, &APongPaddle::Move);
		}
	}
}

void APongPaddle::Move(const FInputActionValue& Value)
{
	CurrentMoveInput = Value.Get<float>();
}

void APongPaddle::ActivateInput() 
{
	UE_LOG(LogTemp, Warning, TEXT("ActivateInput called on %s"), *GetName());

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController valid"));
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Subsystem valid"));
			if (InputMappingContext)
			{
				UE_LOG(LogTemp, Warning, TEXT("InputMappingContext valid, adding it"));
				Subsystem->AddMappingContext(InputMappingContext, 0);
				/* The -> operator is used to access to methods, variables or more, of the object that the pointer is pointing to.
				In this case, Subsystem is a pointer to an object of type UEnhancedInputLocalPlayerSubsystem, and we are calling the AddMappingContext method on that object.*/
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("InputMappingContext is NULL"));
			}
		}
	}
}

// Called every frame
void APongPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentMoveInput != 0.0f)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Y += CurrentMoveInput * MoveSpeed * DeltaTime;
		SetActorLocation(NewLocation);
	}

}

