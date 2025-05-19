#pragma once
#include "Drivers/DragonIdleBodyDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "Drivers/DragonIdleLegDriver.h"
#include "Drivers/DragonIdleWingDriver.h"

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