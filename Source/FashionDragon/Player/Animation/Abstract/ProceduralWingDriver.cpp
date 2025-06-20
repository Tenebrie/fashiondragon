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

	DesiredState = FPoseWingEffector(
		StateData.FlapAngle.CalculateAt(StartingState.FlapAngle, CyclePosition / Duration),
		StateData.TiltAngle.CalculateAt(StartingState.TiltAngle, CyclePosition / Duration),
		StateData.FlightFoldState.CalculateAt(StartingState.FlightFoldState, CyclePosition / Duration),
		StateData.RestFoldState.CalculateAt(StartingState.RestFoldState, CyclePosition / Duration)
	);
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

	return BaseEffector.InterpTo(DesiredState, Context.DeltaTime, State.TransitionSpeed * Context.BlendAlpha);
}
