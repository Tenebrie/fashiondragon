#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonSprintPose;
class FDragonWalkPose;
class FDragonWalkHipSwayDriver;
class FDragonTrotLegDriver;

class FDragonTrotBodyDriver final : public FProceduralBoneDriver
{
	FControlledLeg* LeftLeg;
	FControlledLeg* RightLeg;
	float TargetBlendAlpha = 1.0f;
	FDragonTrotLegDriver* LeadingLeg = nullptr;
	
public:
	FDragonTrotBodyDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* Bone,
		FControlledLeg* LeftLeg,
		FControlledLeg* RightLeg
	): FProceduralBoneDriver(AnimInstance, Bone),
		LeftLeg(LeftLeg), RightLeg(RightLeg)
	{}

	void SetLeadingLeg(FDragonTrotLegDriver* Leg) { LeadingLeg = Leg; }
	virtual void Tick(float DeltaTime) override;
	virtual void SetBlendAlpha(const float NewBlendAlpha) override { TargetBlendAlpha = NewBlendAlpha; }
};

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

	FDragonTrotBodyDriver* BodyDriver;
	FDragonWalkHipSwayDriver* HipsDriver;
	FDragonTrotLegDriver* LeftLegDriver;
	FDragonTrotLegDriver* RightLegDriver;
	void SyncStateFrom(const FDragonWalkPose* SourcePose) const;
	void SyncStateFrom(const FDragonSprintPose* SourcePose) const;
	virtual void ResetState() override;
	virtual void Tick(float DeltaTime) override;
};
