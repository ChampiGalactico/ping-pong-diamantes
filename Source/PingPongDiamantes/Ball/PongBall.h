// Fill out your copyright notice in the Description page of Project Settings.

// Brief theory:
// - Actor: a general object that can be placed in a level. Unlike Pawn, Actors are not necessarily controllable by a PlayerController.
// - APongBall: an Actor representing the game ball. It uses physics for movement and collision, and handles launching, speed control, and bounce behavior.
// - Use GameMode or level placement to spawn this Actor. It holds settings like initial speed, max speed, and sounds for bounces.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongBall.generated.h"

class USoundBase;

UCLASS()
class PINGPONGDIAMANTES_API APongBall : public AActor
{
	GENERATED_BODY()

public:
	APongBall();

	// Visual + physics representation of the ball.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
	UStaticMeshComponent* BallMesh;

	// Starting linear speed applied when launching the ball.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	float InitialSpeed = 600.0f;

	// Cap speed so the ball doesn't accelerate indefinitely.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	float MaxSpeed = 5000.0f;

	// How much to increase the speed each bounce (used in OnHit).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	float SpeedIncreasePerBounce = 50.0f;

	// Delay before the ball is launched on BeginPlay (allows setup time).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	float LaunchDelay = 1.5f;

	// Optional sound to play on bounce events.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	USoundBase* BounceSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	float SoundCooldown = 0.1f;

protected:
	virtual void BeginPlay() override;

	// Launch the ball with a randomized initial direction.
	UFUNCTION()
	void LaunchBall();

	// Called when the ball collides with something. Responsible for speed adjustment and optional sound.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	// Timer handle used to delay the initial launch.
	FTimerHandle LaunchTimerHandle;

	FTimerHandle SoundCooldownTimerHandle;
	bool bCanPlaySound = true;

	UFUNCTION()
	void ResetSoundCooldown();

public:
	virtual void Tick(float DeltaTime) override;
};
