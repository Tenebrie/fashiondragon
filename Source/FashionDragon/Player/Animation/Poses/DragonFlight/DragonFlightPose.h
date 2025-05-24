#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonFlightWingDriver final : public FProceduralWingDriver
{
public:
	FDragonFlightWingDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledWing* ControlledWing
	): FProceduralWingDriver(AnimInstance, ControlledWing) {}

	virtual FDragonWingStateData GetTargetPosition() const override;
};

class FDragonJumpPose final : public FProceduralPose
{
	FDragonFlightWingDriver* LeftWingDriver;
	FDragonFlightWingDriver* RightWingDriver;
public:
	explicit FDragonJumpPose(UDragonAnimInstance* Anim);

	virtual void ResetState() override;
};
