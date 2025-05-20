#include "DragonJumpWingDriver.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonWingStateData FDragonJumpWingDriver::GetTargetPosition() const
{
	return FDragonWingStateData
	{
		.Flap = 0.0f,
		.Openness = 0.0f,
		.Duration = 3.0f,
	};
}

