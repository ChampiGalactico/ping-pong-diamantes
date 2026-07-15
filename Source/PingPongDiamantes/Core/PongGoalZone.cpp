// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGoalZone.h"
#include "Components/BoxComponent.h"
#include "../Ball/PongBall.h"
#include "PongGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APongGoalZone::APongGoalZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GoalBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalBox"));

	RootComponent = GoalBox;

	GoalBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GoalBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void APongGoalZone::BeginPlay()
{
	Super::BeginPlay();
	
	GoalBox->OnComponentBeginOverlap.AddDynamic(this, &APongGoalZone::OnOverlapBegin);
}


void APongGoalZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APongBall* Ball = Cast<APongBall>(OtherActor);

	if (!Ball)
	{
		return;
	}

	if (APongGameMode* GameMode = Cast<APongGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->OnGoalScored(ScoringPlayer, Ball);
	}
}


// Called every frame
void APongGoalZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

