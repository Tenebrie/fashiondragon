#pragma once
#include "FashionDragon/Player/Animation/AbstractProceduralPose.h"

/**
 * @brief Sprint animation leg driver
 */
class FDragonSprintLegDriver final : public FAbstractProceduralLegDriver
{
public:
	FDragonSprintLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual std::pair<FVector, FRotator> GetTargetPosition() const override;
};

/**
 * @brief Sprint animation pose
 */
class FDragonSprintPose final : public FAbstractProceduralPose
{
public:
	explicit FDragonSprintPose(
			UDragonAnimInstance* AnimInstance,
			FControlledLeg* LeftLeg,
			FControlledLeg* RightLeg
		);
};
