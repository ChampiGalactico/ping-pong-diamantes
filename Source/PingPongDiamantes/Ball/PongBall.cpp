// Fill out your copyright notice in the Description page of Project Settings.


#include "PongBall.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APongBall::APongBall()
{
 	// Enable Tick if per-frame logic is needed later.
	PrimaryActorTick.bCanEverTick = true;

	// Create the mesh and use it as the root so physics affects the actor.
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	RootComponent = BallMesh;

	// Configure physics for arcade-style pong behavior:
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetNotifyRigidBodyCollision(true); // so OnComponentHit triggers
	BallMesh->SetUseCCD(true); // continuous collision detection to avoid tunneling at high speeds
	BallMesh->OnComponentHit.AddDynamic(this, &APongBall::OnHit);

	// Disable gravity and reduce damping for predictable movement.
	BallMesh->SetEnableGravity(false);
	BallMesh->SetLinearDamping(0.0f);
	BallMesh->SetAngularDamping(0.0f);

	// Lock Z translation and rotations to constrain movement to the 2D playplane (X,Y).
	BallMesh->BodyInstance.bLockZTranslation = true;
	BallMesh->BodyInstance.bLockXRotation = true;
	BallMesh->BodyInstance.bLockYRotation = true;
}

// Called when the game starts or when spawned
void APongBall::BeginPlay()
{
	// Ensure the ball starts stationary.
	BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

	// Launch after the configured delay (gives time for level setup/possession).
	GetWorldTimerManager().SetTimer(LaunchTimerHandle, this, &APongBall::LaunchBall, LaunchDelay, false);
}

void APongBall::LaunchBall()
{
	// Choose a random angle within a cone and a random horizontal direction (left or right).
	float RandomAngle = FMath::RandRange(-45.0f, 45.0f);
	float DirectionX = (FMath::RandBool() ? 1.0f : -1.0f);

	// Build a direction vector using tangent of the angle for Y component, normalize to keep consistent speed direction.
	FVector Direction = FVector(DirectionX, FMath::Tan(FMath::DegreesToRadians(RandomAngle)), 0.0f).GetSafeNormal();

	// Apply initial linear velocity (physics-driven).
	BallMesh->SetPhysicsLinearVelocity(Direction * InitialSpeed);
}

void APongBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Read current velocity and compute a new speed by adding the configured increase, clamped to MaxSpeed.
	FVector CurrentVelocity = BallMesh->GetPhysicsLinearVelocity();
	float NewSpeed = FMath::Min(CurrentVelocity.Size() + SpeedIncreasePerBounce, MaxSpeed);

	// Keep the current travel direction but ensure it has a minimum horizontal component so the ball doesn't stall vertically.
	FVector Direction = CurrentVelocity.GetSafeNormal();

	// Enforce a minimum horizontal ratio to avoid too-vertical trajectories.
	float MinHorizontalRatio = 0.3f;

	if (FMath::Abs(Direction.X) < MinHorizontalRatio)
	{
		float SignX = (Direction.X >= 0.0f) ? 1.0f : -1.0f;
		Direction.X = SignX * MinHorizontalRatio;
		Direction = Direction.GetSafeNormal();
	}

	// Reapply velocity with the corrected direction and new speed.
	BallMesh->SetPhysicsLinearVelocity(Direction * NewSpeed);

	// Play bounce sound if configured (2D playback so it's heard regardless of location).
	if (BounceSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BounceSound);
	}
}

// Called every frame
void APongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// No per-frame logic required now; any positional limits/clamping could be added here.
}

