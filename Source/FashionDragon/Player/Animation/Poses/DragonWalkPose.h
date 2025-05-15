#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonTrotPose;

class FDragonWalkBodyDriver final : public FProceduralBoneDriver
{
	FControlledLeg* LeftLeg;
	FControlledLeg* RightLeg;
	
public:
	FDragonWalkBodyDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* ControlledBone,
		FControlledLeg* LeftLeg,
		FControlledLeg* RightLeg
	): FProceduralBoneDriver(AnimInstance, ControlledBone),
		LeftLeg(LeftLeg), RightLeg(RightLeg)
	{}

	virtual void Tick(float DeltaTime) override;
	void SyncStateFrom(const FDragonWalkBodyDriver* TargetDriver);
};

class FDragonWalkHipSwayDriver final : public FProceduralBoneDriver
{
	FControlledBone* Hips;
	FControlledLeg* LeftLeg;
	FControlledLeg* RightLeg;
	
public:
	FDragonWalkHipSwayDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* ControlledBone,
		FControlledLeg* LeftLeg,
		FControlledLeg* RightLeg
	): FProceduralBoneDriver(AnimInstance, ControlledBone),
		Hips(ControlledBone), LeftLeg(LeftLeg), RightLeg(RightLeg) {}

	virtual void Tick(float DeltaTime) override;
	void SyncStateFrom(const FDragonWalkHipSwayDriver* TargetDriver);
};

/**
 * @brief Relaxed walk animation leg driver
 */
class FDragonWalkLegDriver final : public FProceduralLegDriver
{
public:
	FDragonWalkLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void AdvanceState() override;
	virtual FDragonWalkStateData GetTargetPosition() const override;
};

/**
 * @brief Relaxed walk animation pose
 */
class FDragonWalkPose final : public FProceduralPose
{
public:
	explicit FDragonWalkPose(UDragonAnimInstance* Anim);

	FDragonWalkBodyDriver* BodyDriver;
	FDragonWalkHipSwayDriver* HipsDriver;
	FDragonWalkLegDriver* LeftLegDriver;
	FDragonWalkLegDriver* RightLegDriver;
	void SyncStateFrom(const FDragonTrotPose* TargetPose) const;
	virtual void ResetState() override;
};