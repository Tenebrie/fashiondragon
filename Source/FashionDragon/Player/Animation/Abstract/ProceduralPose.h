#pragma once
#include "ProceduralBoneDriver.h"
#include "ProceduralLegDriver.h"

class FProceduralBoneDriver;

class FProceduralPose
{
protected:
	UDragonAnimInstance* AnimInstance;

	FProceduralBoneDriver* BodyDriver = nullptr;
	FProceduralBoneDriver* HipsDriver = nullptr;
	TArray<FProceduralLegDriver*> LegDrivers;
	
public:
	virtual ~FProceduralPose() = default;

	explicit FProceduralPose(UDragonAnimInstance* AnimInstance);

	float BlendAlpha = 0.0f;
	virtual void Tick(float DeltaTime);
	virtual void ResetState() {};
	FPoseEffector ToBodyEffector(const FPoseEffector& BaseEffector, const FControlledBone* Body, const float DeltaTime) const;
	FPoseEffector ToHipsEffector(const FPoseEffector& BaseEffector, const FControlledBone* Hips, const float DeltaTime) const;
	FPoseEffector ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const;
	FPoseEffector ToPostProcessLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const;
};
