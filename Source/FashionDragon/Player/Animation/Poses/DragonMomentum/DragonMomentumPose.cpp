#include "DragonMomentumPose.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

FDragonMomentumPose::FDragonMomentumPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LeftLegDriver = new FDragonMomentumDriverLeg(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Momentum));
	RightLegDriver = new FDragonMomentumDriverLeg(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Momentum));
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
}

void FDragonMomentumDriverLeg::Tick(const float DeltaTime)
{
}

FPoseEffector FDragonMomentumDriverLeg::ToEffector(const FPoseEffector& BaseEffector,
                                                   const FPoseEffectorContext& Context)
{
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	for (const auto Layer : *AnimInstance->ControlledLegs[Leg->GetIdx()])
	{
		Position += Layer->Position;
		Rotation += Layer->Rotation;
	}

	// const auto MovementSpeed = AnimInstance->GetCharacter()->GetVelocity().Size();

	float Stiffness = 20.f;
	const auto PlantedPos = Leg->GetPlantedWorldPosition(Position, Rotation, 150.0f);
	if (PlantedPos.GroundHit)
	{
		const float Coefficient = FMath::Clamp(1.0f - PlantedPos.DeltaPosition.Size() / 150.0f, 0.0f, 1.0f);
		Stiffness += 1000.0f * Coefficient;
	}

	const auto WorldPosition = Leg->GetWorldPosition(Position);
	const auto WorldRotation = Leg->GetWorldRotation(Rotation.Quaternion());

	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().Inverse();

	const auto OutputPosition = UKismetMathLibrary::VectorSpringInterp(
		PreviousWorldPosition,
		WorldPosition,
		SpringState,
		Stiffness,
		1.0f,
		Context.DeltaTime
	);

	PreviousWorldPosition = OutputPosition;
	PreviousWorldRotation = WorldRotation;

	return BaseEffector.SetPosition(Transform.GetRotation().RotateVector(OutputPosition - WorldPosition));
}
 