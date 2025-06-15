#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegSteppingDriver.h"

class FDragonFlightDriverLeg final : public FProceduralLegSteppingDriver
{
public:
	FDragonFlightDriverLeg(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
		: FProceduralLegSteppingDriver(AnimInstance, ControlledLeg)
	{
	}

	virtual void Tick(float DeltaTime) override;
};
