#include "DragonDriverTurnToMovement.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

void FDragonDriverTurnToMovement::Tick(const float DeltaTime)
{
	FProceduralBoneDriver::Tick(DeltaTime);

	const auto Rotation = AnimInstance->GetCharacter()->GetMeshActorRotation();
	const auto Diff = Rotation - LastSeenRotation;

	DragDirection += Diff;
	DragDirection = FMath::RInterpTo(DragDirection, FRotator::ZeroRotator, DeltaTime, 5.5f);

	LastSeenRotation = Rotation;
}

FPoseEffector FDragonDriverTurnToMovement::ToEffector(const FPoseEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	FRotator BankRotation = FRotator::ZeroRotator;
	const auto Sensitivity = FMath::Clamp(AnimInstance->GetCharacter()->GetVelocity().Size() / 2000.0f, 0.1f, 1.0f);

	// BankRotation.Pitch = FMath::Clamp(DragDirection.Yaw * Sensitivity * 0.5f, -25.0f, 25.0f) * AxisMask.X * FMath::Sign(DragDirection.Yaw);
	// BankRotation.Yaw = FMath::Clamp(DragDirection.Yaw * Sensitivity * 0.5f, -25.0f, 25.0f) * AxisMask.Y;
	// BankRotation.Roll = FMath::Clamp(DragDirection.Yaw * Sensitivity * 0.5f, -25.0f, 25.0f) * AxisMask.Z;

	// Calculate the base rotation value
	const float BaseRotValue = DragDirection.Yaw * Sensitivity * 0.5f;
	// Apply soft clamp using tanh function (smooth S-curve that approaches ±25 asymptotically)
	const float SoftClampedValue = 25.0f * FMath::Tanh(BaseRotValue / 25.0f);

	BankRotation.Pitch = SoftClampedValue * AxisMask.X * FMath::Sign(DragDirection.Yaw);
	BankRotation.Yaw = SoftClampedValue * AxisMask.Y;
	BankRotation.Roll = SoftClampedValue * AxisMask.Z;
	
	return BaseEffector.SetRotation(BankRotation);
}
