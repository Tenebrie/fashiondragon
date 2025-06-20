#include "DragonJumpWingDriver.h"

FDragonWingStateData FDragonJumpWingDriver::GetRawStateData() const
{
	return FDragonWingStateData
	{
		.FlapAngle = FArticulatedValue(-1.0f),
		.RestFoldState = FArticulatedValue(0.0f),
		.Duration = 0.5f,
		.TransitionSpeed = 5.0f,
	};
}

