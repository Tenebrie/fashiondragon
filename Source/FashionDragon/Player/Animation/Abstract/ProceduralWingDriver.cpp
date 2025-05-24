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
}

void FProceduralWingDriver::ResetState()
{
	CyclePosition = 0.0f;
	VisualCyclePosition = 0.0f;
}

FPoseWingEffector FProceduralWingDriver::ToEffector(const FPoseWingEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto State = GetTargetPosition();
	
	if (Context.BlendAlpha <= 0.0f) { return BaseEffector; }

	return BaseEffector
		.SetFlap(FMath::FInterpTo(BaseEffector.Flap, State.Flap, Context.DeltaTime, State.TransitionSpeed * Context.BlendAlpha))
		.SetOpenness(FMath::FInterpTo(BaseEffector.Openness, State.Openness, Context.DeltaTime, State.TransitionSpeed * Context.BlendAlpha));
}
