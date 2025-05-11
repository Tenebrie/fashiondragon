#pragma once
#include "FashionDragon/Player/Animation/AbstractProceduralDriver.h"
#include "FashionDragon/Player/Animation/AbstractProceduralPose.h"

class FDragonTrotPose;

class FDragonWalkBodyDriver final : public FAbstractProceduralDriver
{
	FControlledLeg* LeftLeg;
	FControlledLeg* RightLeg;
	
public:
	FDragonWalkBodyDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* ControlledBone,
		FControlledLeg* LeftLeg,
		FControlledLeg* RightLeg
	): FAbstractProceduralDriver(AnimInstance, ControlledBone),
		LeftLeg(LeftLeg), RightLeg(RightLeg)
	{}

	virtual void Tick(float DeltaTime) override;
};

class FDragonWalkHipSwayDriver final : public FAbstractProceduralDriver
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
	): FAbstractProceduralDriver(AnimInstance, ControlledBone),
		Hips(ControlledBone), LeftLeg(LeftLeg), RightLeg(RightLeg) {}

	virtual void Tick(float DeltaTime) override;
};

/**
 * @brief Relaxed walk animation leg driver
 */
class FDragonWalkLegDriver final : public FAbstractProceduralLegDriver
{
public:
	FDragonWalkLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void AdvanceState() override;
	virtual std::pair<FVector, FRotator> GetTargetPosition() const override;
};

/**
 * @brief Relaxed walk animation pose
 */
class FDragonWalkPose final : public FAbstractProceduralPose
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