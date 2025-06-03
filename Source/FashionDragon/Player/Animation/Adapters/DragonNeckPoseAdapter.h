#pragma once
#include "FashionDragon/Common/Object.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffector.h"

class UDragonAnimInstance;

class FDragonNeckPoseAdapter final : public FObject
{
	UDragonAnimInstance* Anim;

public:
	explicit FDragonNeckPoseAdapter(UDragonAnimInstance* Anim): Anim(Anim)
	{
	}

	static TArray<FTransform> EffectorToTransforms(const FPoseEffector& Effector);
};
