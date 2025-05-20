#include "ProceduralBoneDriver.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffectorContext.h"

void FProceduralBoneDriver::Tick(const float DeltaTime)
{
	CyclePosition += DeltaTime;
	VisualCyclePosition += DeltaTime;
}

void FProceduralBoneDriver::ResetState()
{
	CyclePosition = 0;
	VisualCyclePosition = 0;
}

FPoseEffector FProceduralBoneDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto PositionDelta = DesiredPosition - BaseEffector.Position;
	const auto RotationDelta = DesiredRotation - BaseEffector.Rotation;

	if (FMath::IsNearlyZero(DesiredForce * Context.BlendAlpha))
	{
		return BaseEffector;
	}

	const auto NewPos = FMath::VInterpTo(BaseEffector.Position, DesiredPosition, Context.DeltaTime, DesiredForce * Context.BlendAlpha);
	const auto NewRot = FMath::RInterpTo(BaseEffector.Rotation, DesiredRotation, Context.DeltaTime, DesiredForce * Context.BlendAlpha);

	const auto Effector = BaseEffector
		.SetPosition(NewPos)
		.SetRotation(NewRot);
	return Effector;
}

FPoseEffector FProceduralBoneDriver::ToPostProcessEffector(const FPoseEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	return BaseEffector;
}
