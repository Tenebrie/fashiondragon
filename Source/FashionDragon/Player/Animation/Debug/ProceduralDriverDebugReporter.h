#pragma once
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

struct FPoseEffector;
struct FDriverDebugInfo;

class FProceduralDriverDebugReporter
{
public:
	virtual ~FProceduralDriverDebugReporter() = default;

	virtual FDriverDebugInfo MakeDebugInfo() const = 0;
};

class FProceduralBoneDriverDebugReporter final : public FProceduralDriverDebugReporter
{
	// Latest known position of the effector
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	// Last applied change to the bone state
	FVector PositionDeltaApplied = FVector::ZeroVector;
	FRotator RotationDeltaApplied = FRotator::ZeroRotator;
public:
	void LogEffectorDelta(const FPoseEffector& BaseEffector, const FPoseEffector& UpdatedEffector);
	virtual FDriverDebugInfo MakeDebugInfo() const override;
};

class FProceduralWingDriverDebugReporter final : public FProceduralDriverDebugReporter
{
	// Latest known state of the effector
	FPoseWingEffector State = FPoseWingEffector();
	// Last applied change to the bone state
	FPoseWingEffector DeltaState = FPoseWingEffector();
public:
	void LogEffectorDelta(const FPoseWingEffector& BaseEffector, const FPoseWingEffector& UpdatedEffector);
	virtual FDriverDebugInfo MakeDebugInfo() const override;
};
