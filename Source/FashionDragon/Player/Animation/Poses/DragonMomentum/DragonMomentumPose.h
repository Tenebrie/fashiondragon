#pragma once

#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "Kismet/KismetMathLibrary.h"

// class FDragonWalkPose;
// class FDragonIdleBodyDriver;
// class FDragonIdleHipsDriver;
// class FDragonIdleLegDriver;
// class FDragonIdleWingDriver;
class FDragonMomentumDriverBone;
class FDragonMomentumDriverLeg;

class FDragonMomentumPose final : public FProceduralPose
{
public:
	explicit FDragonMomentumPose(UDragonAnimInstance* Anim);

	// virtual void Tick(float DeltaTime) override {}

	// FDragonIdleBodyDriver* BodyDriver;
	// FDragonIdleHipsDriver* HipsDriver;
	FDragonMomentumDriverBone* LeftHandDriver;
	FDragonMomentumDriverBone* RightHandDriver;
	FDragonMomentumDriverLeg* LeftLegDriver;
	FDragonMomentumDriverLeg* RightLegDriver;
	// FDragonIdleWingDriver* LeftWingDriver;
	// FDragonIdleWingDriver* RightWingDriver;
};

class FDragonMomentumDriverBone : public FProceduralBoneDriver
{
protected:
	FVector MomentumOffset = FVector::ZeroVector;
	FVectorSpringState SpringState = FVectorSpringState();
	FQuaternionSpringState RotSpringState = FQuaternionSpringState();
	FVector PreviousWorldPosition = FVector::ZeroVector;
	FQuat PreviousWorldRotation = FQuat::Identity;
	
public:
	FDragonMomentumDriverBone(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone) {}

	virtual void NativeBeginPlay() override;
	virtual void ResetState() override;
	FPoseEffector ToEffectorBase(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context,
	                             const FVector& Position, const FRotator& Rotation, const float Stiffness,
	                             const float Damping);
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};

class FDragonMomentumDriverLeg final : public FDragonMomentumDriverBone, public FProceduralLegDriver
{
public:
	FDragonMomentumDriverLeg(UDragonAnimInstance* AnimInstance, FControlledLeg* Leg)
		: FDragonMomentumDriverBone(AnimInstance, Leg), FProceduralLegDriver(AnimInstance, Leg)
	{
	}

	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};