#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonGroundBodySway;
class FDragonGroundTailSway;
class FDragonSprintPose;
class FDragonWalkPose;

class FDragonTrotLegDriver final : public FProceduralLegDriver
{
public:
	FDragonTrotLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void AdvanceState() override;
	virtual FDragonWalkStateData GetRawWalkStateData() const override;
};

/**
 * @brief Fast walk (trot) animation pose
 */
class FDragonTrotPose final : public FProceduralPose
{
public:
	explicit FDragonTrotPose(UDragonAnimInstance* Anim);

	FDragonGroundBodySway* BodyDriver;
	FDragonGroundTailSway* HipsDriver;
	FDragonTrotLegDriver* LeftLegDriver;
	FDragonTrotLegDriver* RightLegDriver;
	void SyncStateFrom(const FDragonWalkPose* SourcePose) const;
	void SyncStateFrom(const FDragonSprintPose* SourcePose) const;
	virtual void ResetState() override;
	virtual void Tick(float DeltaTime) override;
};
