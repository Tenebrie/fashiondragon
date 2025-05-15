#include "ProceduralPose.h"

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
}

FPoseEffector FProceduralPose::ToBodyEffector(const FPoseEffector& BaseEffector, const FControlledBone* Body, const float DeltaTime) const
{
	if (BodyDriver && BodyDriver->GetBone() == Body)
	{
		const auto Context = FPoseEffectorContext(DeltaTime, BlendAlpha);
		return BodyDriver->ToEffector(BaseEffector, Context);
	}
	return BaseEffector;
}

FPoseEffector FProceduralPose::ToHipsEffector(const FPoseEffector& BaseEffector, const FControlledBone* Hips, const float DeltaTime) const
{
	if (HipsDriver && HipsDriver->GetBone() == Hips)
	{
		const auto Context = FPoseEffectorContext(DeltaTime, BlendAlpha);
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
			const auto Context = FPoseEffectorContext(DeltaTime, BlendAlpha);
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
			const auto Context = FPoseEffectorContext(DeltaTime, BlendAlpha);
			return LegDriver->ToPostProcessEffector(BaseEffector, Context);
		}
	}
	return BaseEffector;
}
