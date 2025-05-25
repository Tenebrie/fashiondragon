#include "DragonJumpWingDriver.h"

FDragonWingStateData FDragonJumpWingDriver::GetRawStateData() const
{
	return FDragonWingStateData
	{
		.Flap = FArticulatedValue(-1.0f),
		.Openness = FArticulatedValue(1.0f),
		.Duration = 0.5f,
		.TransitionSpeed = 5.0f,
	};
}

