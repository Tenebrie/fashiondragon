#include "ProceduralBoneDriver.h"

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
	const auto Effector = FPoseEffector(BaseEffector)
		.AddPosition(PositionDelta * Context.BlendAlpha)
		.AddRotation(RotationDelta * Context.BlendAlpha);
	return Effector;
}

FPoseEffector FProceduralBoneDriver::ToPostProcessEffector(const FPoseEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	return BaseEffector;
}
