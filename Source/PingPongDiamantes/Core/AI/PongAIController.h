// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PongAIController.generated.h"

class APongBall;
class APongPaddle;
/**
 * 
 */
UCLASS()
class PINGPONGDIAMANTES_API APongAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AISpeed = 1000.0f;

	void InitializeAI();

	void SetTargetBall(APongBall* NewBall);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	APongPaddle* ControlledPaddle;
	APongBall* TargetBall;
};
