#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonFootPlacementLegDriver final : public FProceduralLegDriver
{
	FRotator LastFrameRotation = FRotator::ZeroRotator;
public:
	FDragonFootPlacementLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
		: FProceduralLegDriver(AnimInstance, ControlledLeg) {}

	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};

class FDragonFootPlacementPose final : public FProceduralPose
{
	FDragonFootPlacementLegDriver* LeftLegDriver;
	FDragonFootPlacementLegDriver* RightLegDriver;
	
public:
	explicit FDragonFootPlacementPose(UDragonAnimInstance* Anim);
};
