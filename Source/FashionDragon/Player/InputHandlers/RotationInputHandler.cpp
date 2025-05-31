#include "RotationInputHandler.h"

#include "InputActionValue.h"

void URotationInputHandler::HandleInput(const FInputActionValue& Value)
{
	const FVector2D MouseMovement = Value.Get<FVector2D>();

	const FQuat DeltaRotation = FRotator(
		MouseMovement.Y * VerticalSensitivity,
		MouseMovement.X * HorizontalSensitivity,
		0.0f
	).Quaternion();

	CurrentRotation = DeltaRotation * CurrentRotation;
}
