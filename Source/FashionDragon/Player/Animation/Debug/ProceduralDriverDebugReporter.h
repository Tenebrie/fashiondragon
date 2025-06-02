#pragma once

struct FPoseEffector;
struct FDriverDebugInfo;

class FProceduralDriverDebugReporter
{
	// Latest known position of the effector
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	// Last applied change to the bone state
	FVector PositionDeltaApplied = FVector::ZeroVector;
	FRotator RotationDeltaApplied = FRotator::ZeroRotator;
public:
	void LogEffectorDelta(const FPoseEffector& BaseEffector, const FPoseEffector& UpdatedEffector);

	FDriverDebugInfo MakeDebugInfo() const;
};
