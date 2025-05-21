#include "DragonIdleWingDriver.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonWingStateData FDragonIdleWingDriver::GetTargetPosition() const
{
	return FDragonWingStateData
	{
		.Flap = 0.0f,
		.Openness = 0.0f,
		.Duration = 3.0f,
		.LinearForce = 1.0f,
	};
}

