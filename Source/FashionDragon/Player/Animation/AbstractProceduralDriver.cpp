#include "AbstractProceduralDriver.h"

FAbstractProceduralDriver::FAbstractProceduralDriver(UDragonAnimInstance* AnimInstance, FControlledBone* ControlledBone): 
	AnimInstance(AnimInstance), Bone(ControlledBone)
{
}

void FAbstractProceduralDriver::Tick(const float DeltaTime)
{
	CyclePosition += DeltaTime;
	VisualCyclePosition += DeltaTime;
}

FPoseEffector FAbstractProceduralDriver::ToEffector(const FPoseEffector& BaseEffector, const float BlendAlpha)
{
	const auto PositionDelta = Position - BaseEffector.Position;
	const auto RotationDelta = Rotation - BaseEffector.Rotation;
	const auto Effector = FPoseEffector{};
	Effector.Position = BaseEffector.Position + PositionDelta * BlendAlpha;
	Effector.Rotation = BaseEffector.Rotation + RotationDelta * BlendAlpha;
	return Effector;
}
