#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonWalkPose;
class FDragonTrotPose;
class FDragonWalkBodyDriver;
class FDragonDriverGroundRootSway;
class FDragonDriverGroundHipSway;

class FDragonSprintLegDriver final : public FProceduralLegDriver
{
public:
	FDragonSprintLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void AdvanceState() override;
	virtual FDragonWalkStateData GetRawWalkStateData() const override;
};

class FDragonSprintPose final : public FProceduralPose
{
public:
	explicit FDragonSprintPose(UDragonAnimInstance* Anim);

	FDragonDriverGroundRootSway* BodyDriver;
	FDragonDriverGroundHipSway* HipsDriver;
	FDragonSprintLegDriver* LeftLegDriver;
	FDragonSprintLegDriver* RightLegDriver;
	void SyncStateFrom(const FDragonWalkPose* SourcePose) const;
	void SyncStateFrom(const FDragonTrotPose* SourcePose) const;
	virtual void ResetState() override;
};
