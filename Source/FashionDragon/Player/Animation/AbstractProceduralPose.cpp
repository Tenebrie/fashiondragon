#include "AbstractProceduralPose.h"

FAbstractProceduralPose::FAbstractProceduralPose(UDragonAnimInstance* AnimInstance):
	AnimInstance(AnimInstance)
{}

void FAbstractProceduralPose::Tick(const float DeltaTime)
{
	for (const auto& LegDriver : LegDrivers)
		LegDriver->Tick(DeltaTime);
}

FPoseEffector FAbstractProceduralPose::ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg) const
{
	for (const auto& LegDriver : LegDrivers)
	{
		if (LegDriver->GetLeg() == Leg)
		{
			return LegDriver->ToEffector(BaseEffector, BlendAlpha);
		}
	}
	return BaseEffector;
}
