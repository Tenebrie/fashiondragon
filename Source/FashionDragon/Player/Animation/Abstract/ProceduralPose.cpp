#include "ProceduralPose.h"

#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

FProceduralPose::FProceduralPose(UDragonAnimInstance* AnimInstance):
	AnimInstance(AnimInstance)
{}

void FProceduralPose::NativeBeginPlay()
{
	for (const auto &BoneDriver : BoneDrivers)
		BoneDriver->NativeBeginPlay();
	for (const auto &LegDriver : LegDrivers)
		LegDriver->NativeBeginPlay();
	for (const auto &WingDriver : WingDrivers)
		WingDriver->NativeBeginPlay();
}

void FProceduralPose::Tick(const float DeltaTime)
{
	for (const auto &BoneDriver : BoneDrivers)
		BoneDriver->Tick(DeltaTime);
	for (const auto &LegDriver : LegDrivers)
		LegDriver->Tick(DeltaTime);
	for (const auto &WingDriver : WingDrivers)
		WingDriver->Tick(DeltaTime);
}

FPoseEffector FProceduralPose::ToBoneEffector(const FPoseEffector& BaseEffector, const FControlledBone* Bone, const float DeltaTime) const
{
	for (const auto& BoneDriver : BoneDrivers)
	{
		if (BoneDriver && BoneDriver->GetBone() == Bone)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, BoneDriver->GetBlendAlpha());
			const auto UpdatedEffector = BoneDriver->ToEffector(BaseEffector, Context);
			BoneDriver->GetDebugReporter()->LogEffectorDelta(BaseEffector, UpdatedEffector);
			return UpdatedEffector;
		}
	}
	return BaseEffector;
}

FPoseEffector FProceduralPose::ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const
{
	for (const auto& LegDriver : LegDrivers)
	{
		if (LegDriver && LegDriver->GetLeg() == Leg)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, LegDriver->GetBlendAlpha());
			const auto UpdatedEffector = LegDriver->ToEffector(BaseEffector, Context);
			LegDriver->GetDebugReporter()->LogEffectorDelta(BaseEffector, UpdatedEffector);
			return UpdatedEffector;
		}
	}
	return BaseEffector;
}

FPoseWingEffector FProceduralPose::ToWingEffector(const FPoseWingEffector& BaseEffector, const FControlledWing* Wing, const float DeltaTime) const
{
	for (const auto& WingDriver : WingDrivers)
	{
		if (WingDriver && WingDriver->GetWing() == Wing)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, WingDriver->GetBlendAlpha());
			const auto UpdatedEffector = WingDriver->ToEffector(BaseEffector, Context);
			return UpdatedEffector;
		}
	}
	return BaseEffector;
}

void FProceduralPose::AddBlendAlpha(const float Delta)
{
	for (FProceduralBoneDriver* Driver : BoneDrivers)
		Driver->SetBlendAlpha(FMath::Clamp(Driver->GetBlendAlpha() + Delta, 0, 1));
	for (FProceduralWingDriver* WingDriver : WingDrivers)
		WingDriver->SetBlendAlpha(FMath::Clamp(WingDriver->GetBlendAlpha() + Delta, 0, 1));
}

void FProceduralPose::SetBlendAlpha(const float BlendAlpha)
{
	for (FProceduralBoneDriver* Driver : BoneDrivers)
		Driver->SetBlendAlpha(BlendAlpha);
	for (FProceduralWingDriver* WingDriver : WingDrivers)
		WingDriver->SetBlendAlpha(BlendAlpha);
}
