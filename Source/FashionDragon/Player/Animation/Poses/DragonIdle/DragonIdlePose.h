#pragma once

#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonWalkPose;
class FDragonTrotPose;
class FDragonSprintPose;
class FDragonIdleBodyDriver;
class FDragonIdleHipsDriver;
class FDragonIdleLegDriver;
class FDragonIdleWingDriver;

class FDragonIdlePose final : public FProceduralPose
{
public:
	explicit FDragonIdlePose(UDragonAnimInstance* Anim);

	virtual void Tick(float DeltaTime) override;

	FDragonIdleBodyDriver* RootDriver;
	FDragonIdleHipsDriver* HipsDriver;
	FDragonIdleLegDriver* LeftLegDriver;
	FDragonIdleLegDriver* RightLegDriver;
	FDragonIdleWingDriver* LeftWingDriver;
	FDragonIdleWingDriver* RightWingDriver;
	
	template<typename DriverT>
	void SyncStateFrom(const DriverT* TargetPose) const;
	virtual void ResetState() override;
};

class FDragonIdleNullDriver final : public FProceduralBoneDriver
{
	FDragonIdleNullDriver(UDragonAnimInstance* AnimInstance, FControlledBone* Bone): FProceduralBoneDriver(AnimInstance, Bone) {}
};