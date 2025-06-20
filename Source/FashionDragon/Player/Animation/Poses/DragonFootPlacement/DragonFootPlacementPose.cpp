#include "DragonFootPlacementPose.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

/**
 * TODO: Figure out the proper IK placement for legs. Trace the shape instead of lines. And rotation.
 */
FPoseEffector FDragonFootPlacementLegDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto GroundData = Leg->GetPlantedWorldPosition(BaseEffector, 5.0f, 300.0f);

	const auto Effector = BaseEffector
		.AddPosition(GroundData.DeltaPosition)
		.AddRotation(GroundData.DeltaRotation.Rotator());
	
	const auto Mesh = AnimInstance->GetSkelMeshComponent();
	const auto Transform = Mesh->GetRelativeTransform();
	const auto SourcePosition = Effector.Position + Leg->BoneOffset;
	const auto SourceRotation = Effector.Rotation;
	const auto LocalPosition = Transform.InverseTransformPosition(SourcePosition) - Leg->BoneOffset;
	const auto LocalRotation = Transform.InverseTransformRotation(FQuat(SourceRotation)).Rotator();

	return Effector.SetPosition(LocalPosition).SetRotation(LocalRotation);
}

FDragonFootPlacementPose::FDragonFootPlacementPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LeftLegDriver = new FDragonFootPlacementLegDriver(Anim, Anim->BackLeftLeg.GetPostProcessBone());
	RightLegDriver = new FDragonFootPlacementLegDriver(Anim, Anim->BackRightLeg.GetPostProcessBone());
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
}
