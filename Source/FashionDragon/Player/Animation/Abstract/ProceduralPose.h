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

	TArray<FProceduralBoneDriver*> RootDrivers;
	TArray<FProceduralBoneDriver*> HipsDrivers;
	TArray<FProceduralLegDriver*> LegDrivers;
	TArray<FProceduralWingDriver*> WingDrivers;
	
public:
	explicit FProceduralPose(UDragonAnimInstance* AnimInstance);

	virtual void NativeBeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void ResetState() {};
	FPoseEffector ToRootEffector(const FPoseEffector& BaseEffector, const FControlledBone* Body, const float DeltaTime) const;
	FPoseEffector ToHipsEffector(const FPoseEffector& BaseEffector, const FControlledBone* Hips, const float DeltaTime) const;
	FPoseEffector ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const;
	FPoseEffector ToPostProcessLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const;
	FPoseWingEffector ToWingEffector(const FPoseWingEffector& BaseEffector, const FControlledWing* Wing, const float DeltaTime) const;

	void AddBlendAlpha(const float Delta);
	void SetBlendAlpha(const float BlendAlpha);

	virtual ~FProceduralPose() = default;
};
