#include "DragonFootPlacementPose.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonFootPlacementLegDriver::FDragonFootPlacementLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
	: FProceduralLegDriver(AnimInstance, ControlledLeg)
{
}

/**
 * TODO: Figure out the proper IK placement for legs. Trace the shape instead of lines. And rotation.
 */
FPoseEffector FDragonFootPlacementLegDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto GroundData = Leg->GetPlantedWorldPosition(BaseEffector);

	return BaseEffector
		.AddPosition(GroundData.DeltaPosition)
		.AddRotation(GroundData.DeltaRotation.Rotator());
}

FPoseEffector FDragonFootPlacementLegDriver::ToPostProcessEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto Mesh = AnimInstance->GetSkelMeshComponent();
	const auto Transform = Mesh->GetRelativeTransform();
	const auto SourcePosition = Leg->Position + Leg->IKBoneOffset;
	const auto SourceRotation = Leg->Rotation;
	const auto LocalPosition = Transform.InverseTransformPosition(SourcePosition) - Leg->IKBoneOffset;
	const auto LocalRotation = Transform.InverseTransformRotation(FQuat(SourceRotation)).Rotator();

	DesiredPosition = LocalPosition;
	DesiredRotation = LocalRotation;
	return BaseEffector.SetPosition(LocalPosition).SetRotation(LocalRotation);
}

FDragonFootPlacementPose::FDragonFootPlacementPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LeftLegDriver = new FDragonFootPlacementLegDriver(Anim, Anim->BackLeftLeg);
	RightLegDriver = new FDragonFootPlacementLegDriver(Anim, Anim->BackRightLeg);
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
}
