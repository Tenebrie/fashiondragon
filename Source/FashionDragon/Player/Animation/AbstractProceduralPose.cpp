#include "AbstractProceduralPose.h"

FAbstractProceduralPose::FAbstractProceduralPose(UDragonAnimInstance* AnimInstance):
	AnimInstance(AnimInstance)
{}

void FAbstractProceduralPose::Tick(const float DeltaTime)
{
	for (const auto& LegDriver : LegDrivers)
		LegDriver->Tick(DeltaTime);
}

FPoseEffector FAbstractProceduralPose::ToLegEffector(const FControlledLeg* Leg) const
{
	const auto Effector = FPoseEffector{};
	for (const auto& LegDriver : LegDrivers)
	{
		if (LegDriver->GetLeg() == Leg)
		{
			Effector.Weight = BlendAlpha;
			Effector.Position = LegDriver->GetPosition();
			Effector.Rotation = LegDriver->GetRotation();
			return Effector;
		}
	}
	return Effector;
}
