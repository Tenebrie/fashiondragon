#include "DragonIdleWingDriver.h"

FDragonWingStateData FDragonIdleWingDriver::GetRawStateData() const
{
	return FDragonWingStateData
	{
		.Flap = FArticulatedValue(),
		.Openness = FArticulatedValue(),
		.Duration = 0.5f,
		.TransitionSpeed = 5.0f,
	};
}

