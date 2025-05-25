#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralWingDriver.h"

class FDragonIdleWingDriver final : public FProceduralWingDriver
{
public:
	FDragonIdleWingDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledWing* ControlledWing
	): FProceduralWingDriver(AnimInstance, ControlledWing) {}

	virtual FDragonWingStateData GetRawStateData() const override;
};
