#pragma once
#include "FashionDragon/Player/Animation/AbstractProceduralPose.h"

class FDragonIdleBodyDriver final : public FAbstractProceduralDriver
{
public:
	FDragonIdleBodyDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* ControlledBone
	): FAbstractProceduralDriver(AnimInstance, ControlledBone) {}
};

/**
 * @brief Idle animation driver
 */
class FDragonIdleLegDriver final : public FAbstractProceduralLegDriver
{
public:
	FDragonIdleLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual std::pair<FVector, FRotator> GetTargetPosition() const override;
};

/**
 * @brief Idle animation pose (no movement)
 */
class FDragonIdlePose final : public FAbstractProceduralPose
{
public:
	explicit FDragonIdlePose(UDragonAnimInstance* Anim);
};