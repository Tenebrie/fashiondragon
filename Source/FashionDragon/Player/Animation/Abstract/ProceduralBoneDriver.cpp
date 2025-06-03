#include "ProceduralBoneDriver.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffectorContext.h"

void FProceduralBoneDriver::Tick(const float DeltaTime)
{
	FBaseDriver::Tick(DeltaTime);
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
	if (FMath::IsNearlyZero(DesiredForce))
	{
		return BaseEffector;
	}

	const auto NewPos = FMath::VInterpTo(BaseEffector.Position, DesiredPosition, Context.DeltaTime, DesiredForce);
	const auto NewRot = FMath::RInterpTo(BaseEffector.Rotation, DesiredRotation, Context.DeltaTime, DesiredForce);

	return BaseEffector
		.SetPosition(NewPos)
		.SetRotation(NewRot);
}
