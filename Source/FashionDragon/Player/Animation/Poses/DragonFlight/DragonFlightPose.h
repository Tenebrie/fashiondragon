#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonFlightWingDriver final : public FProceduralWingDriver
{
public:
	FDragonFlightWingDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledWing* ControlledWing
	): FProceduralWingDriver(AnimInstance, ControlledWing) {}

	// virtual FDragonWingStateData GetTargetPosition() const override;
};

class FDragonFlightPose final : public FProceduralPose
{
	FDragonFlightWingDriver* LeftWingDriver;
	FDragonFlightWingDriver* RightWingDriver;
public:
	explicit FDragonFlightPose(UDragonAnimInstance* Anim);

	virtual void ResetState() override;
};
