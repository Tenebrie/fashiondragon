#include "DragonIdleHipsDriver.h"

void FDragonIdleHipsDriver::Tick(const float DeltaTime)
{
	BlendAlpha = FMath::FInterpTo(BlendAlpha, TargetBlendAlpha, DeltaTime, 5.0f);
	DesiredPosition = FVector::ZeroVector;
	DesiredRotation = FRotator::ZeroRotator;
}
