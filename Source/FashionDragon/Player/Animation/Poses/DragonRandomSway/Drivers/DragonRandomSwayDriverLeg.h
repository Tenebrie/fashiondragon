#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonRandomSwayDriverLeg final : FProceduralPose
{
public:
	explicit FDragonRandomSwayDriverLeg(UDragonAnimInstance* AnimInstance)
		: FProceduralPose(AnimInstance)
	{
	}
};
