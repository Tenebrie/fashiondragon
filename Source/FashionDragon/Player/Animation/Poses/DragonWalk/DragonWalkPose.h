#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegSteppingDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FWalkCyclePoseComponent;
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
public:
	explicit FDragonWalkPose(UDragonAnimInstance* Anim);

	FDragonDriverGroundRootSway* BodyDriver;
	FDragonDriverGroundHipSway* HipsDriver;
	FDragonWalkLegDriver* LeftLegDriver;
	FDragonWalkLegDriver* RightLegDriver;
	FWalkCyclePoseComponent* WalkCycleComponent;
	template<typename SourcePoseT>
	void SyncStateFrom(const SourcePoseT* SourcePose) const;
	virtual void ResetState() override;
};