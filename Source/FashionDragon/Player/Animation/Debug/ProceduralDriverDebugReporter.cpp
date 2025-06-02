#include "ProceduralDriverDebugReporter.h"

#include "AnimationDebugReporter.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffector.h"

void FProceduralDriverDebugReporter::LogEffectorDelta(const FPoseEffector& BaseEffector, const FPoseEffector& UpdatedEffector)
{
	PositionDeltaApplied = UpdatedEffector.Position - BaseEffector.Position;
	RotationDeltaApplied = UpdatedEffector.Rotation - BaseEffector.Rotation;
	Position = UpdatedEffector.Position;
	Rotation = UpdatedEffector.Rotation;
}

FDriverDebugInfo FProceduralDriverDebugReporter::MakeDebugInfo() const
{
	FDriverDebugInfo DriverInfo = FDriverDebugInfo();
	DriverInfo.Position = Position;
	DriverInfo.Rotation = Rotation;
	DriverInfo.PositionDelta = PositionDeltaApplied;
	DriverInfo.RotationDelta = RotationDeltaApplied;
	return DriverInfo;
}
