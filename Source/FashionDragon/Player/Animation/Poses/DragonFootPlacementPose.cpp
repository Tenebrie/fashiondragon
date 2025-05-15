#include "DragonFootPlacementPose.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

#include "DragonIdlePose.h"
#include "FashionDragon/DebugTools/QuickDebug.h"

FDragonFootPlacementLegDriver::FDragonFootPlacementLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
	: FProceduralLegDriver(AnimInstance, ControlledLeg)
{
}

/**
 * TODO: Figure out the proper IK placement for legs. Trace the shape instead of lines as well. And rotation.
 */
FPoseEffector FDragonFootPlacementLegDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	auto FootForward = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().TransformRotation(BaseEffector.Rotation.Quaternion());
	const auto PredictedWorldPosition = Leg->GetWorldPosition(BaseEffector);
	
	FVector Start = PredictedWorldPosition + FVector(0, 0, 300.0f);
	FVector End = PredictedWorldPosition;

	FHitResult HitResultA;
	FHitResult HitResultB;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AnimInstance->GetOwningActor());

	auto FootTip = PredictedWorldPosition + FootForward.RotateVector(FVector(0, 20, 0));

	const bool bHitOnFootBase = AnimInstance->GetWorld()->LineTraceSingleByChannel(
		HitResultA,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	const bool bHitOnFootTip = AnimInstance->GetWorld()->LineTraceSingleByChannel(
		HitResultB,
		FootTip + FVector(0, 0, 300),
		FootTip,
		ECC_Visibility,
		Params
	);

	// DrawDebugLine(AnimInstance->GetWorld(), Start, End, FColor::Red, false, 1.0f);
	// DrawDebugLine(AnimInstance->GetWorld(), FootTip + FVector(0, 0, 100), FootTip, FColor::Green, false, 1.0f);

	// return BaseEffector.SetPosition(FVector(0, 0, 0));
	if (bHitOnFootBase && bHitOnFootTip)
	{
		const FVector GroundPosition = HitResultA.ImpactPoint.Z > HitResultB.ImpactPoint.Z
			? HitResultA.ImpactPoint
			: HitResultB.ImpactPoint;
		const FVector WorldPos = HitResultA.ImpactPoint.Z > HitResultB.ImpactPoint.Z
			? PredictedWorldPosition
			: FootTip;
		return BaseEffector
			.AddPosition((GroundPosition - WorldPos) * 2.0f);
	}
	if (bHitOnFootBase)
	{
		const FVector GroundPosition = HitResultA.ImpactPoint;
		return BaseEffector
			.AddPosition((GroundPosition - PredictedWorldPosition) * 2.0f);
	}
	if (bHitOnFootTip)
	{
		const FVector GroundPosition = HitResultB.ImpactPoint;
		return BaseEffector
			.AddPosition((GroundPosition - FootTip) * 2.0f);
	}
	
	return BaseEffector;
}

FPoseEffector FDragonFootPlacementLegDriver::ToPostProcessEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const auto Mesh = AnimInstance->GetSkelMeshComponent();
	const auto Transform = Mesh->GetRelativeTransform();
	const auto SourcePosition = Leg->Position;
	const auto SourceRotation = Leg->Rotation;
	const auto LocalPosition = Transform.InverseTransformPosition(SourcePosition);
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
