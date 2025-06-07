#include "DragonDriverTurnToMovement.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/InputHandlers/RotationInputHandler.h"

void FDragonDriverTurnToMovement::Tick(const float DeltaTime)
{
	FProceduralBoneDriver::Tick(DeltaTime);

	const auto Rotation = AnimInstance->GetCharacter()->RotationInputHandler->GetRotation();
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

	BankRotation.Pitch = FMath::Clamp(DragDirection.Yaw * Sensitivity * 0.5f, -25.0f, 25.0f) * AxisMask.X * FMath::Sign(DragDirection.Yaw);
	BankRotation.Yaw = FMath::Clamp(DragDirection.Yaw * Sensitivity * 0.5f, -25.0f, 25.0f) * AxisMask.Y;
	BankRotation.Roll = FMath::Clamp(DragDirection.Yaw * Sensitivity * 0.5f, -25.0f, 25.0f) * AxisMask.Z;
	
	return BaseEffector.SetRotation(BankRotation);
}
