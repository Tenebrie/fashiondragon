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

class FControlledLeg : public FControlledBone
{
public:
	FControlledLeg(UDragonAnimInstance* AnimInstance, FName IKBoneName, const FVector& IKBoneOffset, const int Idx);

private:
	UDragonAnimInstance* AnimInstance;
	int Idx;

public:
	FName IKBoneName;
	float MirrorScalar;
	FVector IKBoneOffset = FVector(0.0f, 0.0f, 0.0f);
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector VisualPosition = FVector(0.0f, 0.0f, 0.0f);
	FRotator VisualRotation = FRotator(0.0f, 0.0f, 0.0f);

	FVector GetWorldPosition(const FVector& FromPosition) const;
	FVector GetWorldPosition() const { return GetWorldPosition(Position); }
	FVector GetWorldPosition(const FPoseEffector& FromEffector) const { return GetWorldPosition(FromEffector.Position); }
	
	FQuat GetWorldRotation(const FQuat& FromRotation) const;
	FQuat GetWorldRotation() const { return GetWorldRotation(Rotation.Quaternion()); };
	FQuat GetWorldRotation(const FPoseEffector& FromEffector) const { return GetWorldRotation(FromEffector.Rotation.Quaternion()); }

	FPlantedPositionData GetPlantedWorldPosition(const FVector& AtPosition, const FRotator& AtRotation, const float SweepDown = 0.00f) const;
	FPlantedPositionData GetPlantedWorldPosition(const float SweepDown = 0.00f) const
	{
		return GetPlantedWorldPosition(Position, Rotation, SweepDown);
	};
	FPlantedPositionData GetPlantedWorldPosition(const FPoseEffector& Effector, const float SweepDown = 0.00f) const
	{
		return GetPlantedWorldPosition(Effector.Position, Effector.Rotation, SweepDown);
	};
};
