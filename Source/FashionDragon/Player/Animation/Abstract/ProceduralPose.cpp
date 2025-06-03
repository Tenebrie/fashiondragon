#include "ProceduralPose.h"

#include "ProceduralBoneDriver.h"
#include "ProceduralLegDriver.h"
#include "ProceduralPoseComponent.h"
#include "ProceduralWingDriver.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

void FProceduralPose::NativeBeginPlay()
{
	for (const auto Component : Components)
		Component->NativeBeginPlay();
	for (const auto BoneDriver : BoneDrivers)
		BoneDriver->NativeBeginPlay();
	for (const auto LegDriver : LegDrivers)
		LegDriver->NativeBeginPlay();
	for (const auto WingDriver : WingDrivers)
		WingDriver->NativeBeginPlay();
}

void FProceduralPose::Tick(const float DeltaTime)
{
	for (const auto Component : Components)
		Component->Tick(DeltaTime);
	for (const auto BoneDriver : BoneDrivers)
		BoneDriver->Tick(DeltaTime);
	for (const auto LegDriver : LegDrivers)
		LegDriver->Tick(DeltaTime);
	for (const auto WingDriver : WingDrivers)
		WingDriver->Tick(DeltaTime);
}

FPoseEffector FProceduralPose::ToBoneEffector(const FPoseEffector& BaseEffector, const FControlledBone* Bone, const float DeltaTime) const
{
	for (const auto& BoneDriver : BoneDrivers)
	{
		if (BoneDriver && BoneDriver->GetBone() == Bone)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, 1.0f);
			const auto UpdatedEffector = BoneDriver->ToEffector(BaseEffector, Context);
			const auto BlendedEffector = BaseEffector
				.AddPosition((UpdatedEffector.Position - BaseEffector.Position) * BoneDriver->GetBlendAlpha())
				.AddRotation((UpdatedEffector.Rotation - BaseEffector.Rotation) * BoneDriver->GetBlendAlpha());
			BoneDriver->GetDebugReporter()->LogEffectorDelta(BaseEffector, BlendedEffector);
			return BlendedEffector;
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
			const auto Context = FPoseEffectorContext(DeltaTime, 1.0f);
			const auto UpdatedEffector = LegDriver->ToEffector(BaseEffector, Context);
			const auto BlendedEffector = BaseEffector
				.AddPosition((UpdatedEffector.Position - BaseEffector.Position) * LegDriver->GetBlendAlpha())
				.AddRotation((UpdatedEffector.Rotation - BaseEffector.Rotation) * LegDriver->GetBlendAlpha());
			LegDriver->GetDebugReporter()->LogEffectorDelta(BaseEffector, BlendedEffector);
			return BlendedEffector;
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
			const auto Context = FPoseEffectorContext(DeltaTime, 1.0f);
			const auto UpdatedEffector = WingDriver->ToEffector(BaseEffector, Context);
			// TODO: Blending & reporting
			return UpdatedEffector;
		}
	}
	return BaseEffector;
}

void FProceduralPose::AddBlendAlpha(const float Delta)
{
	for (FProceduralBoneDriver* Driver : BoneDrivers)
		Driver->SetBlendAlpha(FMath::Clamp(Driver->GetBlendAlpha() + Delta, 0, 1));
	for (FProceduralLegDriver* LegDriver : LegDrivers)
		LegDriver->SetBlendAlpha(FMath::Clamp(LegDriver->GetBlendAlpha() + Delta, 0, 1));
	for (FProceduralWingDriver* WingDriver : WingDrivers)
		WingDriver->SetBlendAlpha(FMath::Clamp(WingDriver->GetBlendAlpha() + Delta, 0, 1));
}

void FProceduralPose::SetBlendAlpha(const float BlendAlpha)
{
	for (FProceduralBoneDriver* Driver : BoneDrivers)
		Driver->SetBlendAlpha(BlendAlpha);
	for (FProceduralLegDriver* LegDriver : LegDrivers)
		LegDriver->SetBlendAlpha(BlendAlpha);
	for (FProceduralWingDriver* WingDriver : WingDrivers)
		WingDriver->SetBlendAlpha(BlendAlpha);
}
