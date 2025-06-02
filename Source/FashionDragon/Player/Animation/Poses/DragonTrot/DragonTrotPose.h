#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

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
	FDragonDriverTurnToMovement* BodyDriver;
	FDragonDriverGroundHipSway* HipsDriver;
	FDragonTrotLegDriver* LeftLegDriver;
	FDragonTrotLegDriver* RightLegDriver;
	void SyncStateFrom(const FDragonWalkPose* SourcePose) const;
	void SyncStateFrom(const FDragonSprintPose* SourcePose) const;
	virtual void ResetState() override;
	virtual void Tick(float DeltaTime) override;
};

class FDragonTrotLegDriver final : public FProceduralLegDriver
{
public:
	FDragonTrotLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void AdvanceState() override;
	virtual FDragonWalkStateData GetRawWalkStateData() const override;
};
