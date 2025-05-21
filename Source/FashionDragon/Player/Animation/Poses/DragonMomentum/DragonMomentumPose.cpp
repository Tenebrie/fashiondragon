#include "DragonMomentumPose.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonMomentumPose::FDragonMomentumPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LeftLegDriver = new FDragonMomentumDriverLeg(Anim, Anim->BackLeftLeg);
	RightLegDriver = new FDragonMomentumDriverLeg(Anim, Anim->BackRightLeg);
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
}

FPoseEffector FDragonMomentumDriverLeg::ToEffector(const FPoseEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().Inverse();
	const auto WorldMomentum = Transform.GetRotation().RotateVector(Leg->LinearMomentum);
	Debug::Print(WorldMomentum);
	return BaseEffector.AddPosition(WorldMomentum * Context.DeltaTime);
	// return BaseEffector;
}
