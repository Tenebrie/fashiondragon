#include "AbstractProceduralPose.h"

FAbstractProceduralPose::FAbstractProceduralPose(UDragonAnimInstance* AnimInstance):
	AnimInstance(AnimInstance)
{}

void FAbstractProceduralPose::Tick(const float DeltaTime)
{
	if (BodyDriver)
		BodyDriver->Tick(DeltaTime);
	if (HipsDriver)
		HipsDriver->Tick(DeltaTime);
	
	for (const auto& LegDriver : LegDrivers)
		LegDriver->Tick(DeltaTime);
}

FPoseEffector FAbstractProceduralPose::ToBodyEffector(const FPoseEffector& BaseEffector, const FControlledBone* Body) const
{
	if (BodyDriver && BodyDriver->GetBone() == Body)
	{
		return BodyDriver->ToEffector(BaseEffector, BlendAlpha);
	}
	return BaseEffector;
}

FPoseEffector FAbstractProceduralPose::ToHipsEffector(const FPoseEffector& BaseEffector, const FControlledBone* Hips) const
{
	if (HipsDriver && HipsDriver->GetBone() == Hips)
	{
		return HipsDriver->ToEffector(BaseEffector, BlendAlpha);
	}
	return BaseEffector;
}

FPoseEffector FAbstractProceduralPose::ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg) const
{
	for (const auto& LegDriver : LegDrivers)
	{
		if (LegDriver && LegDriver->GetLeg() == Leg)
		{
			return LegDriver->ToEffector(BaseEffector, BlendAlpha);
		}
	}
	return BaseEffector;
}
