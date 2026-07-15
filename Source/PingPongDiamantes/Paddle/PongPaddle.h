// Fill out your copyright notice in the Description page of Project Settings.
//
// Overview (theory):
// - Pawn: a controllable actor. Pawns represent players or AI-controlled entities.
// - APongPaddle: a Pawn that represents a Pong paddle. It exposes a mesh, movement input and input mapping context.
//   The Pawn receives input from a PlayerController and moves along the Y axis to hit the ball.
//
// Usage:
// - Place the Pawn class in the level or spawn it from a GameMode.
// - Possess it with a PlayerController to allow player input to control movement.
// - InputMappingContext links this Pawn to Enhanced Input mappings (actions/controls).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PongPaddle.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class PINGPONGDIAMANTES_API APongPaddle : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APongPaddle();

	// Visual representation of the paddle.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Paddle")
	UStaticMeshComponent* PaddleMesh;

	// Enhanced Input mapping context to enable specific input bindings for this paddle.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	// Input action used for player 1 movement (Enhanced Input).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction_P1;

	// Movement speed applied when processing input in Tick.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paddle")
	float MoveSpeed = 1000.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Override to bind input actions. Unreal calls this automatically when a PlayerController possesses this Pawn.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Stores the current movement input (-1.0 .. 1.0) set by the Enhanced Input callback.
	float CurrentMoveInput = 0.0f;

public:	
	// Callback bound to the Enhanced Input Move action. Value contains the movement axis amount.
	void Move(const FInputActionValue& Value);

	// Helper to add this Pawn's InputMappingContext to the local player's EnhancedInput subsystem.
	void ActivateInput();

	// Called every frame to apply movement based on CurrentMoveInput.
	virtual void Tick(float DeltaTime) override;

};
