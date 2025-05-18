#pragma once
#include "BodyDriver/DragonIdleBodyDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "LegDriver/DragonIdleLegDriver.h"
#include "WingDriver/DragonIdleWingDriver.h"

class FDragonWalkPose;

class FDragonIdlePose final : public FProceduralPose
{
public:
	explicit FDragonIdlePose(UDragonAnimInstance* Anim);

	virtual void Tick(float DeltaTime) override;

	FDragonIdleBodyDriver* BodyDriver;
	FDragonIdleBodyDriver* HipsDriver;
	FDragonIdleLegDriver* LeftLegDriver;
	FDragonIdleLegDriver* RightLegDriver;
	FDragonIdleWingDriver* LeftWingDriver;
	FDragonIdleWingDriver* RightWingDriver;

	void SyncStateFrom(const FDragonWalkPose* TargetPose) const;
};