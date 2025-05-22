#pragma once
#include "ControlledBone.h"
#include "../Structs/PoseEffector.h"

class UDragonAnimInstance;

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
	FControlledLeg(FControlledLeg&) = default;
	FControlledLeg(UDragonAnimInstance* AnimInstance, FName IKBoneName, const FVector& IKBoneOffset, const int Idx);

private:
	UDragonAnimInstance* AnimInstance;
	int Idx;

	bool IsGrounded = true;

	FVector PreviousWorldPosition = FVector::ZeroVector;
	FQuat PreviousWorldRotation = FQuat::Identity;

public:
	FName IKBoneName;
	float MirrorScalar;
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FVector IKBoneOffset = FVector::ZeroVector;
	FVector LinearMomentum = FVector::ZeroVector;
	FRotator AngularMomentum = FRotator::ZeroRotator;

	virtual void Tick(const float DeltaTime) override;
	
	FVector GetWorldPosition(const FVector& FromPosition) const;
	FVector GetWorldPosition() const { return GetWorldPosition(Position); }
	FVector GetWorldPosition(const FPoseEffector& FromEffector) const { return GetWorldPosition(FromEffector.Position); }
	
	FQuat GetWorldRotation(const FQuat& FromRotation) const;
	FQuat GetWorldRotation() const { return GetWorldRotation(Rotation.Quaternion()); }
	FQuat GetWorldRotation(const FPoseEffector& FromEffector) const { return GetWorldRotation(FromEffector.Rotation.Quaternion()); }

	FPlantedPositionData GetPlantedWorldPosition(const FVector& AtPosition, const FRotator& AtRotation, const float SweepDown = 0.00f) const;
	FPlantedPositionData GetPlantedWorldPosition(const float SweepDown = 0.00f) const
	{
		return GetPlantedWorldPosition(Position, Rotation, SweepDown);
	}
	FPlantedPositionData GetPlantedWorldPosition(const FPoseEffector& Effector, const float SweepDown = 0.00f) const
	{
		return GetPlantedWorldPosition(Effector.Position, Effector.Rotation, SweepDown);
	}
};
