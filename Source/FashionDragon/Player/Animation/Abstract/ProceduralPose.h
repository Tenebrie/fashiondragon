#pragma once
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

#include "FashionDragon/Common/Object.h"
#include "FashionDragon/Player/Animation/Structs/PoseEffector.h"

class FControlledWing;
class FControlledLeg;
class FControlledBone;
class FProceduralWingDriver;
class FProceduralLegDriver;
class FProceduralBoneDriver;
class UDragonAnimInstance;
class FProceduralPoseComponent;

class FProceduralPose : public FObject
{
protected:
	bool Enabled = true;
	
	TArray<FProceduralPoseComponent*> Components;
	TArray<FProceduralBoneDriver*> BoneDrivers;
	TArray<FProceduralLegDriver*> LegDrivers;
	TArray<FProceduralWingDriver*> WingDrivers;
	
public:
	UDragonAnimInstance* AnimInstance;
	explicit FProceduralPose(UDragonAnimInstance* AnimInstance): AnimInstance(AnimInstance) {}

	virtual void NativeBeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void ResetState() {};
	FPoseEffector ToBoneEffector(const FPoseEffector& BaseEffector, const FControlledBone* Bone, const float DeltaTime) const;
	FPoseEffector ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const;
	FPoseWingEffector ToWingEffector(const FPoseWingEffector& BaseEffector, const FControlledWing* Wing, const float DeltaTime) const;

	void SetEnabled(const bool Value) { Enabled = Value; }
	void AddBlendAlpha(const float Delta);
	void SetBlendAlpha(const float BlendAlpha);

	TArray<FProceduralBoneDriver*> ListBoneDrivers() const { return BoneDrivers; }
	TArray<FProceduralLegDriver*> ListLegDrivers() const { return LegDrivers; }
	TArray<FProceduralWingDriver*> ListWingDrivers() const { return WingDrivers; }
};
