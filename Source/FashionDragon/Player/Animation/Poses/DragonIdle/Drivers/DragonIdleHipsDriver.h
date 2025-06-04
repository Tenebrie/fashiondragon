#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FDragonIdleHipsDriver final : public FProceduralBoneDriver
{
	float TargetBlendAlpha = 1.0f;
	float TailSwingCycle = 0.0f;
public:
	FDragonIdleHipsDriver(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* Bone
	): FProceduralBoneDriver(AnimInstance, Bone) {}

	virtual void Tick(float DeltaTime) override;

	virtual void SetDesiredBlendAlpha(const float NewBlendAlpha) override { TargetBlendAlpha = NewBlendAlpha; }
};
