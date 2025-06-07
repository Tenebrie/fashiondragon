#include "DragonIdleBodyDriver.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

void FDragonIdleBodyDriver::Tick(const float DeltaTime)
{
	BlendAlpha = FMath::FInterpTo(BlendAlpha, TargetBlendAlpha, DeltaTime, 5.0f);
	DesiredForce = 20.0f;
	DesiredPosition = FVector(0.0f, 0.0f, 80.0f);
	DesiredRotation = FRotator(20.0f, 0.0f, 0.0f);
}

void FDragonIdleBodyDriver::SetDesiredBlendAlpha(const float NewBlendAlpha)
{
	TargetBlendAlpha = NewBlendAlpha;
}
