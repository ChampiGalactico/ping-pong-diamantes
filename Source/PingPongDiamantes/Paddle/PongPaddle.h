// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Paddle")
	UStaticMeshComponent* PaddleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction_P1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paddle")
	float MoveSpeed = 500.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	float CurrentMoveInput = 0.0f;

public:	

	void Move(const FInputActionValue& Value);

	void ActivateInput();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
