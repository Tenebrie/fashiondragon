#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FControlledLeg;

class FDragonGroundTailSway final : public FProceduralBoneDriver
{
	FControlledBone* Hips;
	FControlledLeg* LeftLeg;
	FControlledLeg* RightLeg;

	float SwayAmplitude = 8.0f;
	
public:
	FDragonGroundTailSway(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* Bone,
		FControlledLeg* LeftLeg,
		FControlledLeg* RightLeg
	): FProceduralBoneDriver(AnimInstance, Bone),
		Hips(Bone), LeftLeg(LeftLeg), RightLeg(RightLeg) {}

	void SetSwayAmplitude(const float Amplitude) { SwayAmplitude = Amplitude; }
	virtual void Tick(float DeltaTime) override;
};
