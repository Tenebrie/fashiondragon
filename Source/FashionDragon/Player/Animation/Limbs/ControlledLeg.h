#pragma once
#include "ControlledBone.h"
#include "../Structs/PoseEffector.h"

class UDragonAnimInstance;

UENUM()
enum class ELegWalkingState
{
	Relaxed,
	Raised,
	Stepping,
	// The leg is moving straight down to the ground
	SeekingGround,
	// The leg's position is locked in world space
	Planted,
	Inertia,
};

struct FPlantedPositionData
{
	bool GroundHit;
	FVector DeltaPosition;
	FQuat DeltaRotation;
};

class FControlledLeg final : public FControlledBone
{
public:
	FControlledLeg(const FControlledLeg& Other) = default;
	FControlledLeg(UDragonAnimInstance* AnimInstance, const FVector& BoneOffset, const int Idx);

private:
	int Idx;

	bool IsGrounded = true;

	FVector PreviousWorldPosition = FVector::ZeroVector;
	FQuat PreviousWorldRotation = FQuat::Identity;

public:
	float MirrorScalar;
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FVector LinearMomentum = FVector::ZeroVector;
	FRotator AngularMomentum = FRotator::ZeroRotator;

	virtual void Tick(const float DeltaTime) override;

	int GetIdx() const { return Idx; }

	FVector GetFlatPosition() const { return FVector(Position.X, Position.Y, 0.0f); }
	FPlantedPositionData GetPlantedWorldPosition(const FVector& AtPosition, const FRotator& AtRotation, const float SweepDown = 0.00f, const float TraceUp = 50.0f) const;
	FPlantedPositionData GetPlantedWorldPosition(const float SweepDown = 0.00f, const float TraceUp = 50.0f) const
	{
		return GetPlantedWorldPosition(Position, Rotation, SweepDown, TraceUp);
	}
	FPlantedPositionData GetPlantedWorldPosition(const FPoseEffector& Effector, const float SweepDown = 0.00f, const float TraceUp = 50.0f) const
	{
		return GetPlantedWorldPosition(Effector.Position, Effector.Rotation, SweepDown, TraceUp);
	}
};
