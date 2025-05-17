#include "BaseUnit.h"

#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	Tick(0.016f);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	GetWorld()->GetTimerManager().SetTimer(
		NavigationUpdateTimer,           // FTimerHandle
		this,                  // Object on which to call the callback
		&ABaseUnit::OnTimerTick,  // Member function to call
		3.0f,                  // Time in seconds
		true                  // false = one-shot, true = looping
	);

	CurrentPathIndex = 0;
	bIsFollowingPath = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->MaxAcceleration = 1000.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
}

void ABaseUnit::OnTimerTick()
{
	const auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) { return; }

	FNavLocation RandomPoint;
	if (!NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), 1500.0f, RandomPoint))
	{
		return;
	}

	const auto Path = NavSys->FindPathSync(FPathFindingQuery(
		this,                  
		*NavSys->MainNavData.Get(), // NavData
		GetActorLocation(),
		RandomPoint
	)).Path.Get();

	const auto Points = Path->GetPathPoints();

	PathPoints.Empty();
	for (const auto& Point : Points)
	{
		PathPoints.Add(Point.Location);
	}
	CurrentPathIndex = 0;
	bIsFollowingPath = true;
}

void ABaseUnit::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFollowingPath && PathPoints.Num() > 0)
	{
		if (CurrentPathIndex < PathPoints.Num())
		{
			// Get current target point
			FVector TargetPoint = PathPoints[CurrentPathIndex];
			FVector Direction = TargetPoint - GetActorLocation();
			// Direction.Z = 0; // Keep movement on the XY plane
            
			const float DistanceToTarget = Direction.Size();
            
			// Check if we've reached the current point
			if (DistanceToTarget < 150.0f)
			{
				CurrentPathIndex++;
                
				// If we've reached the end of the path
				if (CurrentPathIndex >= PathPoints.Num())
				{
					bIsFollowingPath = false;
					return;
				}
			}
			else
			{
				// Move toward the current target point
				Direction.Normalize();
				AddMovementInput(Direction, 1.0f);
                
				// Optional: rotate to face movement direction
				FRotator NewRotation = Direction.Rotation();
				NewRotation.Pitch = 0;
				SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 5.0f));
			}
		}
	}
}
