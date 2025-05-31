#include "DragonDriverGroundHipSway.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

void FDragonDriverGroundHipSway::Tick(const float DeltaTime)
{
	const auto LeftLegOffset = std::min(1.0, LeftLeg->Position.Y / 750.0f);
	const auto RightLegOffset = -std::min(1.0, RightLeg->Position.Y / 750.0f);
	const auto Value = LeftLegOffset + RightLegOffset;

	const auto Sway = FMath::Sin(Value * PI / 2) * 8.0f;
	
	DesiredRotation = FRotator(0.0f, Sway, Sway);
}
