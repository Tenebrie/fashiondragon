#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegSteppingDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonDriverGroundRootSway;
class FDragonDriverGroundHipSway;

class FDragonWalkLegDriver final : public FProceduralLegSteppingDriver
{
public:
	FDragonWalkLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void AdvanceState() override;
	virtual FDragonWalkStateData GetRawWalkStateData() const override;
};

class FDragonWalkPose final : public FProceduralPose
{
	bool SwitchStartingLeg = false;
public:
	explicit FDragonWalkPose(UDragonAnimInstance* Anim);

	FDragonDriverGroundRootSway* BodyDriver;
	FDragonDriverGroundHipSway* HipsDriver;
	FDragonWalkLegDriver* LeftLegDriver;
	FDragonWalkLegDriver* RightLegDriver;
	template<typename DriverT>
	void SyncStateFrom(const DriverT* SourcePose) const;
	virtual void ResetState() override;
};