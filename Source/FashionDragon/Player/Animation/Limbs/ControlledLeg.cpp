#include "ControlledLeg.h"

#include "../DragonAnimInstance.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"

FControlledLeg::FControlledLeg(UDragonAnimInstance* AnimInstance, const FName IKBoneName, const FVector& IKBoneOffset, const int Idx):
	AnimInstance(AnimInstance), Idx(Idx), IKBoneName(IKBoneName), IKBoneOffset(IKBoneOffset)
{
	MirrorScalar = Idx == 1 ? -1.0f : 1.0f;
}

/**
 * @brief Performs a raycast to find the ground position directly under the leg.
 */
FPlantedPositionData FControlledLeg::GetPlantedWorldPosition(const FVector& AtPosition, const FRotator& AtRotation, const float SweepDown) const
{
	const auto FootRotation = GetWorldRotation(AtRotation.Quaternion());
	const auto WorldFootBase = GetWorldPosition(AtPosition);

	// TODO: Fix. As the rotation is not taken into account, this is checking for flat collision.
	const auto WorldFootTip = WorldFootBase + FootRotation.RotateVector(FVector(0, 20, 0));
	// const auto WorldFootTip = WorldFootBase + FVector(0, 20, 0);

	constexpr float TraceDistance = 300.0f;

	FHitResult HitResultA;
	FHitResult HitResultB;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AnimInstance->GetOwningActor());

	const bool bHitOnFootBase = AnimInstance->GetWorld()->LineTraceSingleByChannel(
		HitResultA,
		WorldFootBase + FVector(0, 0, TraceDistance),
		WorldFootBase - FVector(0, 0, SweepDown),
		ECC_Visibility,
		Params
	);

	const bool bHitOnFootTip = AnimInstance->GetWorld()->LineTraceSingleByChannel(
		HitResultB,
		WorldFootTip + FVector(0, 0, TraceDistance),
		WorldFootTip - FVector(0, 0, SweepDown),
		ECC_Visibility,
		Params
	);

	bool GroundHit = true;
	FVector GroundPosition = FVector::ZeroVector;
	FVector FootLocation = FVector::ZeroVector;
	if (bHitOnFootBase && bHitOnFootTip)
	{
		GroundPosition = (HitResultA.ImpactPoint + HitResultB.ImpactPoint) / 2.0f;
		FootLocation = (WorldFootBase + WorldFootTip) / 2.0f;
	}
	else if (bHitOnFootBase)
	{
		GroundPosition = HitResultA.ImpactPoint;
		FootLocation = WorldFootBase;
	}
	else if (bHitOnFootTip)
	{
		GroundPosition = HitResultB.ImpactPoint;
		FootLocation = WorldFootTip;
	}
	else
	{
		GroundHit = false;
		GroundPosition = FVector::ZeroVector;
		FootLocation = FVector::ZeroVector;
	}

	// TODO: Replace `* 2.0f` with actual mesh scale factor/transform multiplication
	return {
		.GroundHit = GroundHit,
		.DeltaPosition = (GroundPosition - FootLocation) * 2.0f,
		.DeltaRotation = FQuat::Identity,
	};
}

void FControlledLeg::Tick(const float DeltaTime)
{
	FControlledBone::Tick(DeltaTime);
	if (DeltaTime < KINDA_SMALL_NUMBER) { return; }

	if (CanProduceEvents)
	{
		const auto PlantedPos = GetPlantedWorldPosition(2.0f);
		if (PlantedPos.GroundHit && !IsGrounded)
		{
			AnimInstance->GetCharacter()->OnLegPlanted.Broadcast(GetWorldPosition());
		}
		IsGrounded = PlantedPos.GroundHit;
	}

	const auto WorldPosition = GetWorldPosition();
	const auto WorldRotation = GetWorldRotation();

	constexpr float MomentumSmoothTime = 2.5f;
	constexpr float MomentumDampingCoefficient = 0.2f;
	
	const FVector DeltaPos = WorldPosition - PreviousWorldPosition;
	const FVector LinearVelocity = DeltaPos / DeltaTime;

	const float Alpha = 1.f - FMath::Exp(-DeltaTime / MomentumSmoothTime);
	LinearMomentum = FMath::Lerp(LinearMomentum, LinearVelocity, Alpha);

	const float DecayFactor = FMath::Exp(-MomentumDampingCoefficient * DeltaTime);
	LinearMomentum *= DecayFactor;
	
	PreviousWorldPosition = WorldPosition;
	PreviousWorldRotation = WorldRotation;
}

FVector FControlledLeg::GetWorldPosition(const FVector& FromPosition) const
{
	const auto ComponentTransform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform();
	return ComponentTransform.TransformPosition(FromPosition + IKBoneOffset);
}

FQuat FControlledLeg::GetWorldRotation(const FQuat& FromRotation) const
{
	const auto ComponentTransform = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentTransform();
	return ComponentTransform.TransformRotation(FromRotation);
}
