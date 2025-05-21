#pragma once

#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

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
public:
	FDragonMomentumDriverLeg(UDragonAnimInstance* AnimInstance, FControlledLeg* Leg)
		: FProceduralLegDriver(AnimInstance, Leg)
	{
	}

	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
	virtual void AdvanceState() override {}
	virtual FDragonWalkStateData GetRawWalkStateData() const override { return {}; }
};