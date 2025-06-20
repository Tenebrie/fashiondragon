#include "DragonIdleWingDriver.h"

FDragonWingStateData FDragonIdleWingDriver::GetRawStateData() const
{
	return FDragonWingStateData
	{
		.RestFoldState = FArticulatedValue(1.0f),
		.Duration = 0.5f,
		.TransitionSpeed = 5.0f,
	};
}

