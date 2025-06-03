#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegSteppingDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FWalkCyclePoseComponent;
class FDragonDriverGroundRootSway;
class FDragonDriverTurnToMovement;
class FDragonDriverGroundHipSway;
class FDragonSprintPose;
class FDragonWalkPose;

class FDragonTrotLegDriver;

/**
 * @brief Default run animation pose.
 */
class FDragonTrotPose final : public FProceduralPose
{
	
public:
	explicit FDragonTrotPose(UDragonAnimInstance* Anim);

	FDragonDriverGroundRootSway* RootDriver;
	FDragonDriverGroundHipSway* HipsDriver;
	FDragonTrotLegDriver* LeftLegDriver;
	FDragonTrotLegDriver* RightLegDriver;
	FWalkCyclePoseComponent* WalkCycleComponent;
	void SyncStateFrom(const FDragonWalkPose* SourcePose) const;
	void SyncStateFrom(const FDragonSprintPose* SourcePose) const;
	virtual void ResetState() override;
};

class FDragonTrotLegDriver final : public FProceduralLegSteppingDriver
{
public:
	FDragonTrotLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void AdvanceState() override;
	virtual FDragonWalkStateData GetRawWalkStateData() const override;
};
