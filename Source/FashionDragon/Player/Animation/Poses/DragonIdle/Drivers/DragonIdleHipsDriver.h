#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FDragonIdleHipsDriver final : public FProceduralBoneDriver
{
	float TargetBlendAlpha = 1.0f;
	float TailSwingCycle = 0.0f;
public:
	FDragonIdleHipsDriver(
		UDragonAnimInstance* AnimInstance,
		TArray<FControlledBone*>* ControlledBones,
		const int DriverGroup
	): FProceduralBoneDriver(AnimInstance, ControlledBones, DriverGroup) {}

	virtual void Tick(float DeltaTime) override;

	virtual void SetBlendAlpha(const float NewBlendAlpha) override { TargetBlendAlpha = NewBlendAlpha; }
};
