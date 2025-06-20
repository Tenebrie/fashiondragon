#include "ProceduralDriverDebugReporter.h"

#include "AnimationDebugReporter.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffector.h"

void FProceduralBoneDriverDebugReporter::LogEffectorDelta(const FPoseEffector& BaseEffector, const FPoseEffector& UpdatedEffector)
{
	PositionDeltaApplied = UpdatedEffector.Position - BaseEffector.Position;
	RotationDeltaApplied = UpdatedEffector.Rotation - BaseEffector.Rotation;
	Position = UpdatedEffector.Position;
	Rotation = UpdatedEffector.Rotation;
}

FDriverDebugInfo FProceduralBoneDriverDebugReporter::MakeDebugInfo() const
{
	FDriverDebugInfo DriverInfo = FDriverDebugInfo();
	DriverInfo.Position = Position;
	DriverInfo.Rotation = Rotation;
	DriverInfo.PositionDelta = PositionDeltaApplied;
	DriverInfo.RotationDelta = RotationDeltaApplied;
	return DriverInfo;
}

void FProceduralWingDriverDebugReporter::LogEffectorDelta(const FPoseWingEffector& BaseEffector, const FPoseWingEffector& UpdatedEffector)
{
	State = UpdatedEffector;
	DeltaState = UpdatedEffector - BaseEffector;
}

FDriverDebugInfo FProceduralWingDriverDebugReporter::MakeDebugInfo() const
{
	FDriverDebugInfo DriverInfo = FDriverDebugInfo();
	DriverInfo.IsWing = true;
	DriverInfo.WingState = State;
	DriverInfo.WingDelta = DeltaState;
	return DriverInfo;
}

