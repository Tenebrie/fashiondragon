#include "ControlledLeg.h"

#include "../DragonAnimInstance.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"

FControlledLeg::FControlledLeg(UDragonAnimInstance* AnimInstance, const FVector& BoneOffset, const int Idx):
	FControlledBone(AnimInstance), Idx(Idx)
{
	MirrorScalar = Idx == 1 ? -1.0f : 1.0f;
	FControlledBone::BoneOffset = BoneOffset;
}

FPlantedPositionData FControlledLeg::GetSimplePlantedWorldPosition(const FVector& AtPosition,
	const FRotator& AtRotation, const float SweepDown, const float TraceUp) const
{
	const auto WorldFootBase = GetWorldPosition(AtPosition);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AnimInstance->GetOwningActor());

	const bool bHitOnFootBase = AnimInstance->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WorldFootBase + FVector(0, 0, TraceUp),
		WorldFootBase - FVector(0, 0, SweepDown),
		ECC_Visibility,
		Params
	);
	
	if (bHitOnFootBase)
	{
		return {
			.GroundHit = true,
			.DeltaPosition = (HitResult.ImpactPoint - WorldFootBase) / AnimInstance->GetCharacter()->GetMeshActorScale().Z,
			.DeltaRotation = FQuat::Identity,
		};
	}

	return {
		.GroundHit = false,
		.DeltaPosition = FVector::ZeroVector,
		.DeltaRotation = FQuat::Identity,
	};
}

FPlantedPositionData FControlledLeg::GetDetailedPlantedWorldPosition(const FVector& AtPosition,
                                                                     const FRotator& AtRotation, const float SweepDown, const float TraceUp) const
{
	const auto OldFootRotation = GetWorldRotation(AtRotation.Quaternion());
	const auto WorldFootBase = GetWorldPosition(AtPosition);

	const auto WorldFootTip = WorldFootBase + OldFootRotation.RotateVector(FVector(40, 0, 0));

	FHitResult HitResultA;
	FHitResult HitResultB;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AnimInstance->GetOwningActor());

	const bool bHitOnFootBase = AnimInstance->GetWorld()->LineTraceSingleByChannel(
		HitResultA,
		WorldFootBase + FVector(0, 0, TraceUp),
		WorldFootBase - FVector(0, 0, SweepDown),
		ECC_Visibility,
		Params
	);

	const bool bHitOnFootTip = AnimInstance->GetWorld()->LineTraceSingleByChannel(
		HitResultB,
		WorldFootTip + FVector(0, 0, TraceUp),
		WorldFootTip - FVector(0, 0, SweepDown),
		ECC_Visibility,
		Params
	);

	bool GroundHit = true;
	FVector GroundPosition;
	FVector FootLocation;
	FQuat FootRotation;
	if (bHitOnFootBase)
	{
		GroundPosition = HitResultA.ImpactPoint;
		FootLocation = WorldFootBase;

		FVector Normal = FVector(HitResultA.Normal.X, HitResultA.Normal.Y, HitResultA.Normal.Z);
		FVector Forward = OldFootRotation.Vector() - FVector::DotProduct(OldFootRotation.Vector(), Normal) * Normal;
		FRotator TargetRotator = FRotationMatrix::MakeFromXZ(Forward, Normal).Rotator();
		FootRotation = TargetRotator.Quaternion();
	}
	else if (bHitOnFootTip)
	{
		GroundHit = false;
		FVector Normal = FVector(HitResultB.Normal.X, HitResultB.Normal.Y, HitResultB.Normal.Z);
		FVector Forward = OldFootRotation.Vector() - FVector::DotProduct(OldFootRotation.Vector(), Normal) * Normal;
		FRotator TargetRotator = FRotationMatrix::MakeFromXZ(Forward, Normal).Rotator();
		FootRotation = TargetRotator.Quaternion();
		GroundPosition = FVector::ZeroVector;
		FootLocation = FVector::ZeroVector;
	}
	else
	{
		GroundHit = false;
		GroundPosition = FVector::ZeroVector;
		FootLocation = FVector::ZeroVector;
		FootRotation = OldFootRotation;
	}
	return {
		.GroundHit = GroundHit,
		.DeltaPosition = (GroundPosition - FootLocation) / AnimInstance->GetCharacter()->GetMeshActorScale().Z,
		.DeltaRotation = (FootRotation.Rotator() - OldFootRotation.Rotator()).Quaternion(),
	};
}

/**
 * @brief Performs a raycast to find the ground position directly under the leg.
 */
FPlantedPositionData FControlledLeg::GetPlantedWorldPosition(const FVector& AtPosition, const FRotator& AtRotation, const float SweepDown, const float TraceUp) const
{
	const auto SimpleHit = GetSimplePlantedWorldPosition(AtPosition, AtRotation, SweepDown + 75.0f, TraceUp);
	if (!SimpleHit.GroundHit)
	{
		return SimpleHit;
	}
	return GetDetailedPlantedWorldPosition(AtPosition, AtRotation, SweepDown, TraceUp);
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
			const auto ActualPosition = AnimInstance->GetSkelMeshComponent()->GetComponentTransform().TransformPosition(AnimInstance->LegPositions[Idx] + BoneOffset);
			AnimInstance->GetCharacter()->OnLegPlanted.Broadcast(ActualPosition);
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
