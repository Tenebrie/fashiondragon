#pragma once

#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonWalkPose;
class FDragonIdleBodyDriver;
class FDragonIdleHipsDriver;
class FDragonIdleLegDriver;
class FDragonIdleWingDriver;

class FDragonIdlePose final : public FProceduralPose
{
public:
	explicit FDragonIdlePose(UDragonAnimInstance* Anim);

	virtual void Tick(float DeltaTime) override;

	FDragonIdleBodyDriver* BodyDriver;
	FDragonIdleHipsDriver* HipsDriver;
	FDragonIdleLegDriver* LeftLegDriver;
	FDragonIdleLegDriver* RightLegDriver;
	FDragonIdleWingDriver* LeftWingDriver;
	FDragonIdleWingDriver* RightWingDriver;

	void SyncStateFrom(const FDragonWalkPose* TargetPose) const;
	virtual void ResetState() override;
};