#pragma once
#include "Structs/PoseEffector.h"

class UDragonAnimInstance;

enum class ELegWalkingState
{
	Relaxed,
	Raised,
	Stepping,
	Planted,
	Inertia,
};

class FControlledBone
{
public:
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
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
	FVector IKBoneOffset = FVector(0.0f, 0.0f, 0.0f);
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector VisualPosition = FVector(0.0f, 0.0f, 0.0f);
	FRotator VisualRotation = FRotator(0.0f, 0.0f, 0.0f);

	FVector GetWorldPosition() const;
	FVector GetWorldPosition(const FPoseEffector& WithEffector) const;
};
