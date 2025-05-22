#include "DragonMomentumPose.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonMomentumPose::FDragonMomentumPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LeftLegDriver = new FDragonMomentumDriverLeg(Anim, Anim->BackLeftLeg.GetBone(EBodyDriverLayer::Momentum));
	RightLegDriver = new FDragonMomentumDriverLeg(Anim, Anim->BackRightLeg.GetBone(EBodyDriverLayer::Momentum));
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
		
	// return BaseEffector.SetPosition(WorldMomentum / 10.0f);
	return BaseEffector;
}
