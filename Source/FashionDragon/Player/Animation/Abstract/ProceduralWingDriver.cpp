#include "ProceduralWingDriver.h"

FDragonWingStateData FProceduralWingDriver::GetTargetPosition() const
{
	return FDragonWingStateData { .Duration = 0.5f };
}

FProceduralWingDriver::FProceduralWingDriver(UDragonAnimInstance* AnimInstance, FControlledWing* ControlledWing): 
	AnimInstance(AnimInstance), Wing(ControlledWing)
{
}

void FProceduralWingDriver::Tick(const float DeltaTime)
{
	const auto State = GetTargetPosition();

	const auto AdvanceValue = Inverted ? -DeltaTime : DeltaTime;
	
	CyclePosition = std::min(CycleDuration, CyclePosition + AdvanceValue);
	VisualCyclePosition = std::min(State.Duration, VisualCyclePosition + AdvanceValue);
	if (CyclePosition >= 1.0f)
	{
		Inverted = true;
		CyclePosition = 1.0f;
		VisualCyclePosition = 1.0f;
	}
	else if (CyclePosition <= 0.0f)
	{
		Inverted = false;
		CyclePosition = 0.0f;
		VisualCyclePosition = 0.0f;
	}
}

FPoseWingEffector FProceduralWingDriver::ToEffector(const FPoseWingEffector& BaseEffector,
                                                    const FPoseEffectorContext& Context)
{
	return BaseEffector;
}
