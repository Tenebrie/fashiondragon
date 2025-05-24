#include "DragonJumpWingDriver.h"

FDragonWingStateData FDragonJumpWingDriver::GetTargetPosition() const
{
	return FDragonWingStateData
	{
		.Flap = -1.0f,
		.Openness = 1.0f,
		.Duration = 5.5f,
		.TransitionSpeed = 2.0f,
	};
}

