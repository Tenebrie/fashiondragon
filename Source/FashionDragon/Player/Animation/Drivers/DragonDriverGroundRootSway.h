#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"

class FProceduralLegDriver;
class FControlledLeg;

class FDragonDriverGroundRootSway final : public FProceduralBoneDriver
{
	FControlledLeg* LeftLeg;
	FControlledLeg* RightLeg;
	float TargetBlendAlpha = 1.0f;
	FProceduralLegDriver* LeadingLeg = nullptr;

	float HorizontalAmplitude = 10.0f;
	float VerticalOffset = 100.0f;
	float VerticalAmplitude = 175.0f;
	
public:
	FDragonDriverGroundRootSway(
		UDragonAnimInstance* AnimInstance,
		FControlledBone* Bone,
		FControlledLeg* LeftLeg,
		FControlledLeg* RightLeg
	): FProceduralBoneDriver(AnimInstance, Bone),
		LeftLeg(LeftLeg), RightLeg(RightLeg)
	{}

	void SetLeadingLeg(FProceduralLegDriver* Leg) { LeadingLeg = Leg; }
	void SetHorizontalAmplitude(const float Amplitude) { HorizontalAmplitude = Amplitude; }
	void SetVerticalOffset(const float Offset) { VerticalOffset = Offset; }
	void SetVerticalAmplitude(const float Amplitude) { VerticalAmplitude = Amplitude; }
	virtual void Tick(float DeltaTime) override;
	virtual void SetBlendAlpha(const float NewBlendAlpha) override { TargetBlendAlpha = NewBlendAlpha; }
};
