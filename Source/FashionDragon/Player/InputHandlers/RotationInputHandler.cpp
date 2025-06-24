#include "RotationInputHandler.h"

#include "InputActionValue.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"

void URotationInputHandler::HandleMovementInput(const FInputActionValue& Value)
{
	const FVector2D TargetMovement = Value.Get<FVector2D>();
	UserInput = TargetMovement;

	const FRotator Rotation = GetCharacter()->Controller->GetControlRotation();
	const FRotator YawRot(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
	const FVector WorldMovement = ForwardDirection * TargetMovement.Y + RightDirection * TargetMovement.X;
	MovementRotation = WorldMovement.Rotation();
	SmoothMovementRotation = FMath::VInterpTo(
		SmoothMovementRotation,
		MovementRotation.Vector(),
		GetWorld()->GetDeltaSeconds(),
		1.0f
	);
}

void URotationInputHandler::HandleRotationInput(const FInputActionValue& Value)
{
	const FVector2D MouseMovement = Value.Get<FVector2D>();

	const float DeltaPitch = MouseMovement.Y * VerticalSensitivity;
	const float DeltaYaw = MouseMovement.X * HorizontalSensitivity;

	CurrentPitch += DeltaPitch;
	CurrentYaw += DeltaYaw;
}

void URotationInputHandler::HandleFlightRollInput(const FInputActionValue& Value)
{
	const float DeltaValue = Value.Get<float>();

	CurrentRoll += DeltaValue * RollSensitivity;
}
