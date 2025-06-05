#include "DragonMomentumPose.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
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

void FDragonMomentumDriverLeg::NativeBeginPlay()
{
	FProceduralLegDriver::NativeBeginPlay();
	
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	for (const auto Layer : *AnimInstance->ControlledLegs[Leg->GetIdx()])
	{
		Position += Layer->Position;
		Rotation += Layer->Rotation;
	}
	
	PreviousWorldPosition = Leg->GetWorldPosition(Position);
	PreviousWorldRotation = Leg->GetWorldRotation(Rotation.Quaternion());
}

void FDragonMomentumDriverLeg::Tick(const float DeltaTime)
{
}

void FDragonMomentumDriverLeg::ResetState()
{
	FProceduralLegDriver::ResetState();
	MomentumOffset = FVector::ZeroVector;
	PreviousWorldPosition = Leg->GetWorldPosition();
	PreviousWorldRotation = Leg->GetWorldRotation();
	PreviousWorldPosition = FVector::ZeroVector;
	PreviousWorldRotation = FQuat::Identity;
	SpringState = FVectorSpringState();
	SpringStateVertical = FVectorSpringState();
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

	float Stiffness = 20.f;
	const auto PlantedPos = Leg->GetPlantedWorldPosition(Position, Rotation, 150.0f);
	if (PlantedPos.GroundHit)
	{
		const float Coefficient = FMath::Clamp(1.0f - PlantedPos.DeltaPosition.Size() / 150.0f, 0.0f, 1.0f);
		Stiffness += 1000.0f * Coefficient;
	}

	const auto WorldPosition = Leg->GetWorldPosition(Position);
	const auto WorldRotation = Leg->GetWorldRotation(Rotation.Quaternion());

	if (FVector::DistSquared(WorldPosition, PreviousWorldPosition) > 150000.0f)
	{
		ResetState();
		PreviousWorldPosition = WorldPosition;
		PreviousWorldRotation = WorldRotation;
	}

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
 