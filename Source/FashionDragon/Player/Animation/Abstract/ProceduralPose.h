#pragma once
#include "ProceduralBoneDriver.h"
#include "ProceduralLegDriver.h"
#include "ProceduralWingDriver.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

class FProceduralBoneDriver;

class FProceduralPose
{
protected:
	UDragonAnimInstance* AnimInstance;

	FProceduralBoneDriver* BodyDriver = nullptr;
	FProceduralBoneDriver* HipsDriver = nullptr;
	TArray<FProceduralLegDriver*> LegDrivers;
	TArray<FProceduralWingDriver*> WingDrivers;
	
public:
	explicit FProceduralPose(UDragonAnimInstance* AnimInstance);

	virtual void Tick(float DeltaTime);
	virtual void ResetState() {};
	FPoseEffector ToBodyEffector(const FPoseEffector& BaseEffector, const FControlledBone* Body, const float DeltaTime) const;
	FPoseEffector ToHipsEffector(const FPoseEffector& BaseEffector, const FControlledBone* Hips, const float DeltaTime) const;
	FPoseEffector ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const;
	FPoseEffector ToPostProcessLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const;
	FPoseWingEffector ToWingEffector(const FPoseWingEffector& BaseEffector, const FControlledWing* Wing, const float DeltaTime) const;

	void AddBlendAlpha(const float Delta);
	void SetBlendAlpha(const float BlendAlpha);

	virtual ~FProceduralPose() = default;
};
