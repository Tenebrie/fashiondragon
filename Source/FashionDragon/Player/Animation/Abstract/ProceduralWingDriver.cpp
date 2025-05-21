#include "ProceduralWingDriver.h"

#include "FashionDragon/DebugTools/QuickDebug.h"

FDragonWingStateData FProceduralWingDriver::GetTargetPosition() const
{
	return FDragonWingStateData { .Duration = 0.5f };
}

FProceduralWingDriver::FProceduralWingDriver(UDragonAnimInstance* AnimInstance, FControlledWing* ControlledWing): 
	FBaseDriver(AnimInstance), Wing(ControlledWing)
{
}

void FProceduralWingDriver::Tick(const float DeltaTime)
{
	const auto State = GetTargetPosition();

	const auto AdvanceValue = Inverted ? -DeltaTime : DeltaTime;

	CycleDuration = State.Duration;
	CyclePosition = std::min(CycleDuration, CyclePosition + AdvanceValue);
	VisualCyclePosition = std::min(State.Duration, VisualCyclePosition + AdvanceValue);
	if (CyclePosition >= State.Duration)
	{
		Inverted = true;
		CyclePosition = State.Duration;
		VisualCyclePosition = State.Duration;
	}
	else if (CyclePosition <= 0.0f)
	{
		Inverted = false;
		CyclePosition = 0.0f;
		VisualCyclePosition = 0.0f;
	}
}

FPoseWingEffector FProceduralWingDriver::ToEffector(const FPoseWingEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto State = GetTargetPosition();
	
	const auto LinearSpeed = State.LinearForce * Context.BlendAlpha * Context.DeltaTime;

	const auto FlapDistToMove = std::min(LinearSpeed, FMath::Abs(State.Flap - Wing->Flap));
	const auto OpennessDistToMove = std::min(LinearSpeed, FMath::Abs(State.Openness - Wing->Openness));
	
	return BaseEffector
		.AddFlap(FlapDistToMove * FMath::Sign(State.Flap - Wing->Flap))
		.AddOpenness(OpennessDistToMove * FMath::Sign(State.Openness - Wing->Openness));
}
