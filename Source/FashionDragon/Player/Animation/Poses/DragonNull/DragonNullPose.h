#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonNullPose final : public FProceduralPose
{
public:
	explicit FDragonNullPose(UDragonAnimInstance* Anim);

	void EvaluateBlending();
};

class FDragonNullDriverBone final : public FProceduralBoneDriver
{
public:
	FDragonNullDriverBone(UDragonAnimInstance* AnimInstance, FControlledBone* Bone): FProceduralBoneDriver(AnimInstance, Bone) {}
};

class FDragonNullDriverLeg final : public FProceduralLegDriver
{
public:
	FDragonNullDriverLeg(UDragonAnimInstance* AnimInstance, FControlledLeg* Leg): FProceduralLegDriver(AnimInstance, Leg) {}
};
