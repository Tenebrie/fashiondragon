#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FDragonIdleBodyDriver final : public FProceduralBoneDriver
{
public:
	FDragonIdleBodyDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* ControlledBone
	): FProceduralBoneDriver(AnimInstance, ControlledBone) {}
};
