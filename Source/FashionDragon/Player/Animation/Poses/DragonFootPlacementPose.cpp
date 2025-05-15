#include "DragonFootPlacementPose.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

#include "DragonIdlePose.h"
#include "FashionDragon/DebugTools/QuickDebug.h"

FDragonFootPlacementLegDriver::FDragonFootPlacementLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg)
	: FProceduralLegDriver(AnimInstance, ControlledLeg)
{
}

FPoseEffector FDragonFootPlacementLegDriver::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	// const auto CurrentOffset = BaseEffector.Position - Leg->Position;
	const auto PredictedWorldPosition = Leg->GetWorldPosition(BaseEffector);

	// DrawDebugLine(AnimInstance->GetWorld(), FVector(0, 0, 0), PredictedWorldPosition, FColor::Red, false, 1.0f);

	FVector LegWorldPosition = PredictedWorldPosition;
	FVector Start = LegWorldPosition + FVector(0, 0, 20.0f);
	FVector End = Start - FVector(0, 0, 15.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AnimInstance->GetOwningActor());

	FVector BoxExtent = FVector(20.f, 30.f, 10.f);
	FQuat Rotation = (BaseEffector.Rotation + AnimInstance->GetSkelMeshComponent()->GetComponentRotation()).Quaternion();

	if (GEngine && GEngine->IsEditor())
	{
		// Draw the box at start position
		DrawDebugBox(
			AnimInstance->GetWorld(),
			Start,
			BoxExtent,
			Rotation,
			FColor::Green,
			false,
			0.0f,
			0,
			1.0f
		);

		// Draw the box at end position
		DrawDebugBox(
			AnimInstance->GetWorld(),
			End,
			BoxExtent,
			Rotation,
			FColor::Blue,
			false,
			0.0f,
			0,
			1.0f
		);

		// Draw the trace line
		DrawDebugLine(
			AnimInstance->GetWorld(),
			Start,
			End,
			FColor::Yellow,
			false,
			0.0f,
			0,
			1.0f
		);
	}

	const bool bHit = AnimInstance->GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		Rotation,
		ECC_Visibility,
		FCollisionShape::MakeBox(BoxExtent),
		Params
	);

	if (!bHit)
	{
		return BaseEffector;
	}

	DrawDebugPoint(
				AnimInstance->GetWorld(),
				HitResult.ImpactPoint,
				10.0f,
				FColor::Red,
				false,
				0.0f
			);
            
	// Draw the surface normal
	DrawDebugLine(
		AnimInstance->GetWorld(),
		HitResult.ImpactPoint,
		HitResult.ImpactPoint + HitResult.Normal * 30.0f,
		FColor::Red,
		false,
		0.0f,
		0,
		2.0f
	);

	FVector GroundPosition = HitResult.ImpactPoint;
	const auto Offset = GroundPosition - PredictedWorldPosition;

	// Rotation adjustment to align with surface normal
	FVector UpVector = FVector(0, 0, 1);
	FVector SurfaceNormal = HitResult.Normal;
        
	// Calculate rotation to align foot with surface
	FQuat DeltaRotation = FQuat::FindBetweenNormals(UpVector, SurfaceNormal);
        
	// Apply rotation adjustment to foot
	// Blend factor controls how much we conform to the surface (0.0-1.0)
	float BlendFactor = 0.75f; 
	FQuat BlendedRotation = FQuat::Slerp(FQuat::Identity, DeltaRotation, BlendFactor);

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
	// const auto Effector = FPoseEffector();
	// Effector.Position = LocalPosition;
	// Effector.Rotation = LocalRotation;
	return BaseEffector;
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
