#include "DragonIdleWingDriver.h"

FDragonWingStateData FDragonIdleWingDriver::GetTargetPosition() const
{
	return FDragonWingStateData
	{
		.Flap = CyclePosition * 2.0f - 1.0f,
		.Openness = 0.0f,
		.Duration = 1.0f,
	};
}

FPoseWingEffector FDragonIdleWingDriver::ToEffector(const FPoseWingEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	const auto Target = GetTargetPosition();
	return FPoseWingEffector(Target.Flap, Target.Openness);
}
