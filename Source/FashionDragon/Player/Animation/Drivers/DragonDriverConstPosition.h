#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

enum class EDriverConstPositionMode
{
	Additive,
	Absolute
};

class FDragonDriverConstPosition final : public FProceduralBoneDriver
{
public:
	FDragonDriverConstPosition(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
		BlendMode = EDriverBlend::EaseOut;
		BlendSpeed = 0.5f;
	}

	EDriverConstPositionMode AdditionMode = EDriverConstPositionMode::Additive;
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
