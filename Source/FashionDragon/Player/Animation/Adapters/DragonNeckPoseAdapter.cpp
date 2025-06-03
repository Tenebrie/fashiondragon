#include "DragonNeckPoseAdapter.h"

TArray<FTransform> FDragonNeckPoseAdapter::EffectorToTransforms(const FPoseEffector& Effector)
{
	constexpr float HeadFraction = 1.0f;
	constexpr float UpperNeckFraction = 0.7f;
	constexpr float LowerNeckFraction = 0.3f;

	const FRotator HeadRotation = FRotator(
		Effector.Rotation.Pitch * HeadFraction,
		Effector.Rotation.Yaw * HeadFraction,
		Effector.Rotation.Roll * HeadFraction);
	const FRotator UpperNeckRotation = FRotator(
		Effector.Rotation.Pitch * UpperNeckFraction,
		Effector.Rotation.Yaw * UpperNeckFraction,
		Effector.Rotation.Roll * UpperNeckFraction);
	const FRotator LowerNeckRotation = FRotator(
		Effector.Rotation.Pitch * LowerNeckFraction,
		Effector.Rotation.Yaw * LowerNeckFraction,
		Effector.Rotation.Roll * LowerNeckFraction);

	return {
		FTransform(HeadRotation, FVector::ZeroVector),
		FTransform(UpperNeckRotation, FVector::ZeroVector),
		FTransform(LowerNeckRotation, FVector::ZeroVector)
	};
}
