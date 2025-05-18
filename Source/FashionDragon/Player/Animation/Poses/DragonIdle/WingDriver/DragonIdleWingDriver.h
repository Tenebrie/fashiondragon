#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralWingDriver.h"

class FDragonIdleWingDriver final : public FProceduralWingDriver
{
public:
	FDragonIdleWingDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledWing* ControlledWing
	): FProceduralWingDriver(AnimInstance, ControlledWing) {}

	virtual FDragonWingStateData GetTargetPosition() const override;
	virtual FPoseWingEffector ToEffector(const FPoseWingEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
