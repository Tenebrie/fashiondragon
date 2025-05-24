#include "DragonIdleWingDriver.h"

FDragonWingStateData FDragonIdleWingDriver::GetTargetPosition() const
{
	return FDragonWingStateData
	{
		.Flap = 0.0f,
		.Openness = 0.0f,
		.Duration = 3.0f,
		.TransitionSpeed = 2.0f,
	};
}

