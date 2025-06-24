#include "RotationInputHandler.h"

#include "FlightHandler.h"
#include "InputActionValue.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"

void URotationInputHandler::HandleGroundRotationInput(const FInputActionValue& Value)
{
	FVector2D MouseMovement = Value.Get<FVector2D>();

	MouseMovement = MouseMovement.GetRotated(-CurrentRoll);

	const float DeltaPitch = MouseMovement.Y * VerticalSensitivity;
	const float DeltaYaw = MouseMovement.X * HorizontalSensitivity;
	
	CurrentPitch += DeltaPitch;
	CurrentYaw += DeltaYaw;
}

void URotationInputHandler::HandleFlightRotationInput(const FInputActionValue& Value)
{
	const FVector2D MouseMovement = Value.Get<FVector2D>();

	const float DeltaPitch =  -MouseMovement.Y * VerticalSensitivity;
	const float DeltaYaw   =  MouseMovement.X * HorizontalSensitivity;

	FQuat Orientation = FQuat( FRotator(CurrentPitch, CurrentYaw, CurrentRoll) );

	const FVector LocalRight = Orientation.GetRightVector();
	const FVector LocalUp    = Orientation.GetUpVector();

	Orientation = FQuat(LocalRight, FMath::DegreesToRadians(DeltaPitch)) * Orientation;
	Orientation = FQuat(LocalUp,    FMath::DegreesToRadians(DeltaYaw))   * Orientation;

	const FRotator NewRot = Orientation.Rotator();
	CurrentPitch = NewRot.Pitch;
	CurrentYaw   = NewRot.Yaw;
	CurrentRoll  = NewRot.Roll;
}

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
}

void URotationInputHandler::HandleRotationInput(const FInputActionValue& Value)
{
	if (GetCharacter()->FlightHandler->IsFlying())
		HandleFlightRotationInput(Value);
	else
		HandleGroundRotationInput(Value);
}

void URotationInputHandler::HandleFlightRollInput(const FInputActionValue& Value)
{
	const float DeltaValue = Value.Get<float>();
	const float DeltaRoll  = -DeltaValue * RollSensitivity;

	FQuat Orientation = FQuat( FRotator(CurrentPitch, CurrentYaw, CurrentRoll) );

	const FVector LocalForward = Orientation.GetForwardVector();
	Orientation = FQuat(LocalForward, FMath::DegreesToRadians(DeltaRoll)) * Orientation;

	const FRotator NewRot = Orientation.Rotator();
	CurrentPitch = NewRot.Pitch;
	CurrentYaw   = NewRot.Yaw;
	CurrentRoll  = NewRot.Roll;
}

void URotationInputHandler::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetCharacter()->FlightHandler->IsFlying())
	{
		CurrentRoll = FMath::FInterpTo(CurrentRoll, 0.0f, DeltaTime, 5.0f);
	}
	else
	{
		MovementRotation = FRotator(0.0f, CurrentYaw, 0.0f);
	}
}
