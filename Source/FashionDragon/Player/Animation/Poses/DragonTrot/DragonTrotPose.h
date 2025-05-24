#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonWalkPose;
class FDragonWalkBodyDriver;
class FDragonWalkHipSwayDriver;

/**
 * @brief Fast walk (trot) animation leg driver
 */
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

	// FDragonWalkBodyDriver* BodyDriver;
	// FDragonWalkHipSwayDriver* HipsDriver;
	FDragonTrotLegDriver* LeftLegDriver;
	FDragonTrotLegDriver* RightLegDriver;
	void SyncStateFrom(const FDragonWalkPose* TargetPose) const;
	virtual void ResetState() override;
};
