#include "DragonMomentumPose.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/InputHandlers/FlightHandler.h"

FDragonMomentumPose::FDragonMomentumPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LeftHandDriver = new FDragonMomentumDriverBone(Anim, Anim->LeftHand.GetBone(EDriverLayer::Momentum));
	RightHandDriver = new FDragonMomentumDriverBone(Anim, Anim->RightHand.GetBone(EDriverLayer::Momentum));
	LeftLegDriver = new FDragonMomentumDriverLeg(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Momentum));
	RightLegDriver = new FDragonMomentumDriverLeg(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Momentum));
	BoneDrivers = {
		LeftHandDriver,
		RightHandDriver,
	};
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
}

void FDragonMomentumDriverBone::ResetState()
{
	FProceduralBoneDriver::ResetState();
	MomentumOffset = FVector::ZeroVector;
	PreviousWorldPosition = Bone->GetWorldPosition();
	PreviousWorldRotation = Bone->GetWorldRotation();
	PreviousWorldPosition = FVector::ZeroVector;
	PreviousWorldRotation = FQuat::Identity;
	SpringState = FVectorSpringState();
	RotSpringState = FQuaternionSpringState();
}

void FDragonMomentumDriverBone::NativeBeginPlay()
{
	FProceduralBoneDriver::NativeBeginPlay();
	
	PreviousWorldPosition = Bone->GetWorldPosition();
	PreviousWorldRotation = Bone->GetWorldRotation();
}

FPoseEffector FDragonMomentumDriverBone::ToEffectorBase(const FPoseEffector& BaseEffector,
                                                        const FPoseEffectorContext& Context, const FVector& Position,
                                                        const FRotator& Rotation, const float Stiffness, const float Damping)
{
	const auto WorldPosition = Bone->GetWorldPosition(Position);
	// const auto WorldRotation = Bone->GetWorldRotation(Rotation.Quaternion());

	if (FVector::DistSquared(WorldPosition, PreviousWorldPosition) > 150000.0f)
	{
		ResetState();
		PreviousWorldPosition = WorldPosition;
		// PreviousWorldRotation = WorldRotation;
	}

	const auto Transform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform().Inverse();

	const auto OutputPosition = UKismetMathLibrary::VectorSpringInterp(
		PreviousWorldPosition,
		WorldPosition,
		SpringState,
		Stiffness,
		Damping,
		Context.DeltaTime
	);
	//
	// const auto OutputRotation = UKismetMathLibrary::QuaternionSpringInterp(
	// 	PreviousWorldRotation,
	// 	WorldRotation,
	// 	RotSpringState,
	// 	Stiffness,
	// 	1.0f,
	// 	Context.DeltaTime
	// );

	PreviousWorldPosition = OutputPosition;
	// PreviousWorldRotation = OutputRotation;

	return BaseEffector
		.SetPosition(Transform.GetRotation().RotateVector(OutputPosition - WorldPosition));
}

FPoseEffector FDragonMomentumDriverBone::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	const auto BoneGroup = AnimInstance->GetBoneGroup(Bone->GroupName);
	if (!BoneGroup)
		return BaseEffector;

	for (const auto Layer : *BoneGroup)
	{
		Position += Layer->Position;
		Rotation += Layer->Rotation;
	}

	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = Bone->GetWorldRotation().UnrotateVector(OwningActor->GetVelocity());
	
	const auto PreProcessEffector = ToEffectorBase(BaseEffector, Context, Position, Rotation, 400.0f, 0.7f);

	float Stiffness = 35.0f;
	if (OwningActor->FlightHandler->IsFlying())
	{
		Stiffness += 100.0f;
	}
	const auto Effector = PreProcessEffector
		.SetPosition(FVector(PreProcessEffector.Position.X, PreProcessEffector.Position.Y, PreProcessEffector.Position.Z))
		.AddPosition(-MovementSpeed / Stiffness);

	constexpr float Coefficient = 0.1f;
	const auto TargetEffector = Effector.SetRotation(FRotator(Effector.Position.X * Coefficient, Effector.Position.Y * Coefficient, 0.0f));

	return BaseEffector
		.SetPosition(FMath::VInterpTo(BaseEffector.Position, TargetEffector.Position, Context.DeltaTime, 45.0f))
		.SetRotation(FMath::RInterpTo(BaseEffector.Rotation, TargetEffector.Rotation, Context.DeltaTime, 45.0f));
}

FPoseEffector FDragonMomentumDriverLeg::ToEffector(const FPoseEffector& BaseEffector,
                                                   const FPoseEffectorContext& Context)
{
	FVector Position = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	for (const auto Layer : *FDragonMomentumDriverBone::AnimInstance->ControlledLegs[Leg->GetIdx()])
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
	if (FDragonMomentumDriverBone::AnimInstance->GetCharacter()->FlightHandler->IsFlying())
	{
		Stiffness += 100.0f;
	}

	return ToEffectorBase(BaseEffector, Context, Position, Rotation, Stiffness, 1.0f);
}
 