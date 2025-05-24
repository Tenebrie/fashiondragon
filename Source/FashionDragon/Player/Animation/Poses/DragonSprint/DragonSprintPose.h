#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonWalkPose;
class FDragonWalkBodyDriver;
class FDragonWalkHipSwayDriver;

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
	
	FDragonSprintLegDriver* LeftLegDriver;
	FDragonSprintLegDriver* RightLegDriver;
	void SyncStateFrom(const FDragonWalkPose* TargetPose) const;
	virtual void ResetState() override;
};
