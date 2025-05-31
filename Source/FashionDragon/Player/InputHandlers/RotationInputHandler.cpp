#include "RotationInputHandler.h"

#include "InputActionValue.h"

void URotationInputHandler::HandleInput(const FInputActionValue& Value)
{
	const FVector2D MouseMovement = Value.Get<FVector2D>();

	const float DeltaPitch = MouseMovement.Y * VerticalSensitivity;
	const float DeltaYaw = MouseMovement.X * HorizontalSensitivity;

	CurrentPitch += DeltaPitch;
	CurrentYaw += DeltaYaw;
}
