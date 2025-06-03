#include "ProceduralWingDriver.h"

#include "Curves/CurveEvaluation.h"
#include "FashionDragon/DebugTools/QuickDebug.h"

FDragonWingStateData FProceduralWingDriver::GetRawStateData() const
{
	return FDragonWingStateData { .Duration = 0.5f };
}

void FProceduralWingDriver::RecalculatePose()
{
	const auto StateData = GetRawStateData();
	auto Duration = StateData.Duration;
	if (FMath::Abs(Duration) < 0.001f)
		Duration = 0.001f;

	DesiredFlap = UE::Curves::BezierInterp(
			StartedFlapFrom,
			StartedFlapFrom + StateData.Flap.StartArticulation,
			StateData.Flap.Value + StateData.Flap.EndArticulation,
			StateData.Flap.Value,
			CyclePosition / Duration
		);

	DesiredOpenness = UE::Curves::BezierInterp(
			StartedOpennessFrom,
			StartedOpennessFrom + StateData.Openness.StartArticulation,
			StateData.Openness.Value + StateData.Openness.EndArticulation,
			StateData.Openness.Value,
			CyclePosition / Duration
		);
}

FProceduralWingDriver::FProceduralWingDriver(UDragonAnimInstance* AnimInstance, FControlledWing* ControlledWing): 
	FBaseDriver(AnimInstance), Wing(ControlledWing)
{
}

void FProceduralWingDriver::Tick(const float DeltaTime)
{
	FBaseDriver::Tick(DeltaTime);
	const auto State = GetRawStateData();

	const auto AdvanceValue = DeltaTime;

	CyclePosition = std::min(State.Duration, CyclePosition + AdvanceValue);

	RecalculatePose();
}

void FProceduralWingDriver::ResetState()
{
	CyclePosition = 0.0f;
}

FPoseWingEffector FProceduralWingDriver::ToEffector(const FPoseWingEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto State = GetRawStateData();
	
	if (Context.BlendAlpha <= 0.0f) { return BaseEffector; }

	return BaseEffector
		.SetFlap(FMath::FInterpTo(BaseEffector.Flap, DesiredFlap, Context.DeltaTime, State.TransitionSpeed * Context.BlendAlpha))
		.SetOpenness(FMath::FInterpTo(BaseEffector.Openness, DesiredOpenness, Context.DeltaTime, State.TransitionSpeed * Context.BlendAlpha));
}
