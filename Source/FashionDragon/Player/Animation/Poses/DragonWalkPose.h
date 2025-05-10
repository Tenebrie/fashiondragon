#pragma once
#include "FashionDragon/Player/Animation/AbstractProceduralPose.h"

class FDragonTrotPose;

/**
 * @brief Relaxed walk animation leg driver
 */
class FDragonWalkLegDriver final  : public FAbstractProceduralLegDriver
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
	explicit FDragonWalkPose(
			UDragonAnimInstance* AnimInstance,
			FControlledLeg* LeftLeg,
			FControlledLeg* RightLeg
		);

	FDragonWalkLegDriver* LeftLegDriver;
	FDragonWalkLegDriver* RightLegDriver;
	void SyncStateFrom(const FDragonTrotPose* TargetPose) const;
	virtual void ResetState() override;
};