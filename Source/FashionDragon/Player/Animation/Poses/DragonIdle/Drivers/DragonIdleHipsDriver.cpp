#include "DragonIdleHipsDriver.h"

void FDragonIdleHipsDriver::Tick(const float DeltaTime)
{
	BlendAlpha = FMath::FInterpTo(BlendAlpha, TargetBlendAlpha, DeltaTime, 5.0f);
	DesiredPosition = FVector::ZeroVector;
	DesiredRotation = FRotator::ZeroRotator;
	TailSwingCycle += DeltaTime;
	const auto BreathCyclePosition = FMath::Sin(TailSwingCycle * 0.5f * PI);
	DesiredRotation = FRotator(0, BreathCyclePosition * 5.0f, 0);
}
