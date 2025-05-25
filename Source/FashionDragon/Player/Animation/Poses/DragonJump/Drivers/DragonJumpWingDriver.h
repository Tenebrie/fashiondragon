#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralWingDriver.h"

class FDragonJumpWingDriver final : public FProceduralWingDriver
{
public:
	FDragonJumpWingDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledWing* ControlledWing
	): FProceduralWingDriver(AnimInstance, ControlledWing) {}

	virtual FDragonWingStateData GetRawStateData() const override;
};
