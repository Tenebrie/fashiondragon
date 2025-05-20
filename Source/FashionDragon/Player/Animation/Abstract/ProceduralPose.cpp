#include "ProceduralPose.h"

#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

FProceduralPose::FProceduralPose(UDragonAnimInstance* AnimInstance):
	AnimInstance(AnimInstance)
{}

void FProceduralPose::Tick(const float DeltaTime)
{
	if (BodyDriver)
		BodyDriver->Tick(DeltaTime);
	if (HipsDriver)
		HipsDriver->Tick(DeltaTime);
	
	for (const auto& LegDriver : LegDrivers)
		LegDriver->Tick(DeltaTime);

	for (const auto& WingDriver : WingDrivers)
		WingDriver->Tick(DeltaTime);
}

FPoseEffector FProceduralPose::ToBodyEffector(const FPoseEffector& BaseEffector, const FControlledBone* Body, const float DeltaTime) const
{
	if (BodyDriver && BodyDriver->GetBone() == Body)
	{
		const auto Context = FPoseEffectorContext(DeltaTime, BodyDriver->GetBlendAlpha());
		return BodyDriver->ToEffector(BaseEffector, Context);
	}
	return BaseEffector;
}

FPoseEffector FProceduralPose::ToHipsEffector(const FPoseEffector& BaseEffector, const FControlledBone* Hips, const float DeltaTime) const
{
	if (HipsDriver && HipsDriver->GetBone() == Hips)
	{
		const auto Context = FPoseEffectorContext(DeltaTime, HipsDriver->GetBlendAlpha());
		return HipsDriver->ToEffector(BaseEffector, Context);
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
			return LegDriver->ToEffector(BaseEffector, Context);
		}
	}
	return BaseEffector;
}

FPoseEffector FProceduralPose::ToPostProcessLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const
{
	for (const auto& LegDriver : LegDrivers)
	{
		if (LegDriver && LegDriver->GetLeg() == Leg)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, LegDriver->GetBlendAlpha());
			return LegDriver->ToPostProcessEffector(BaseEffector, Context);
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
			return WingDriver->ToEffector(BaseEffector, Context);
		}
	}
	return BaseEffector;
}

void FProceduralPose::AddBlendAlpha(const float Delta)
{
	if (BodyDriver)
		BodyDriver->SetBlendAlpha(FMath::Clamp(BodyDriver->GetBlendAlpha() + Delta, 0, 1));
	if (HipsDriver)
		HipsDriver->SetBlendAlpha(FMath::Clamp(HipsDriver->GetBlendAlpha() + Delta, 0, 1));
	for (FProceduralLegDriver* LegDriver : LegDrivers)
		LegDriver->SetBlendAlpha(FMath::Clamp(LegDriver->GetBlendAlpha() + Delta, 0, 1));
	for (FProceduralWingDriver* WingDriver : WingDrivers)
		WingDriver->SetBlendAlpha(FMath::Clamp(WingDriver->GetBlendAlpha() + Delta, 0, 1));
}

void FProceduralPose::SetBlendAlpha(const float BlendAlpha)
{
	if (BodyDriver)
		BodyDriver->SetBlendAlpha(BlendAlpha);
	if (HipsDriver)
		HipsDriver->SetBlendAlpha(BlendAlpha);
	for (FProceduralLegDriver* LegDriver : LegDrivers)
		LegDriver->SetBlendAlpha(BlendAlpha);
	for (FProceduralWingDriver* WingDriver : WingDrivers)
		WingDriver->SetBlendAlpha(BlendAlpha);
}
