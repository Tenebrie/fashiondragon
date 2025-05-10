#pragma once
#include "FashionDragon/Player/Animation/AbstractProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/AbstractProceduralPose.h"

/**
 * @brief Fast walk (trot) animation leg driver
 */
class FDragonTrotLegDriver final : public FAbstractProceduralLegDriver
{
public:
	FDragonTrotLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg);

	virtual void Tick(float DeltaTime) override;
	virtual void AdvanceState() override;
	virtual std::pair<FVector, FRotator> GetTargetPosition() const override;
};

/**
 * @brief Fast walk (trot) animation pose
 */
class FDragonTrotPose final : public FAbstractProceduralPose
{
public:
	explicit FDragonTrotPose(
			UDragonAnimInstance* AnimInstance,
			FControlledLeg* LeftLeg,
			FControlledLeg* RightLeg
		);
};
