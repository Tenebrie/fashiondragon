#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FDragonIdleBodyDriver final : public FProceduralBoneDriver
{
	float TargetBlendAlpha = 1.0f;
public:
	FDragonIdleBodyDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* Bone
	): FProceduralBoneDriver(AnimInstance, Bone) {}

	virtual void Tick(float DeltaTime) override;

	virtual void SetBlendAlpha(const float NewBlendAlpha) override;
};
