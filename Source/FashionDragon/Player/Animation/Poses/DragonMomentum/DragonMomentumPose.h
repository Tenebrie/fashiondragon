#pragma once

#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "Kismet/KismetMathLibrary.h"

// class FDragonWalkPose;
// class FDragonIdleBodyDriver;
// class FDragonIdleHipsDriver;
// class FDragonIdleLegDriver;
// class FDragonIdleWingDriver;
class FDragonMomentumDriverLeg;

class FDragonMomentumPose final : public FProceduralPose
{
public:
	explicit FDragonMomentumPose(UDragonAnimInstance* Anim);

	// virtual void Tick(float DeltaTime) override {}

	// FDragonIdleBodyDriver* BodyDriver;
	// FDragonIdleHipsDriver* HipsDriver;
	FDragonMomentumDriverLeg* LeftLegDriver;
	FDragonMomentumDriverLeg* RightLegDriver;
	// FDragonIdleWingDriver* LeftWingDriver;
	// FDragonIdleWingDriver* RightWingDriver;
};

class FDragonMomentumDriverLeg final : public FProceduralLegDriver
{
	FVector MomentumOffset = FVector::ZeroVector;
	FVectorSpringState SpringState = FVectorSpringState();
	FVectorSpringState SpringStateVertical = FVectorSpringState();
	FVector PreviousWorldPosition = FVector::ZeroVector;
	FQuat PreviousWorldRotation = FQuat::Identity;
public:
	FDragonMomentumDriverLeg(UDragonAnimInstance* AnimInstance, FControlledLeg* Leg)
		: FProceduralLegDriver(AnimInstance, Leg)
	{
	}

	virtual void NativeBeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};