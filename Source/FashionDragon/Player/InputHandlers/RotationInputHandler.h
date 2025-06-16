#pragma once
#include "FashionDragon/Player/Generics/PlayerComponent.h"
#include "RotationInputHandler.generated.h"

struct FInputActionValue;

UCLASS()
class URotationInputHandler final : public UPlayerComponent
{
	GENERATED_BODY()

	FVector2D UserInput = FVector2D::ZeroVector;
	FVector2D WorldUserInput = FVector2D::ZeroVector;
	FRotator MovementRotation = FRotator::ZeroRotator;
	FVector SmoothMovementRotation = FVector::ZeroVector;

	float HorizontalSensitivity = 1.0f;
	float VerticalSensitivity = 1.0f;

	float CurrentPitch = 0.0f;
	float CurrentYaw = 0.0f;
	float CurrentRoll = 0.0f;
	
public:
	FRotator GetInputForwardWorldRotation() const { return MovementRotation; }
	FVector GetSmoothInputForwardWorldRotation() const { return SmoothMovementRotation; }
	FRotator GetCameraWorldRotation() const { return FRotator(CurrentPitch, CurrentYaw, CurrentRoll); }
	void ResetRotation(const FQuat& Rotation)
	{
		CurrentPitch = Rotation.Rotator().Pitch;
		CurrentYaw = Rotation.Rotator().Yaw;
		CurrentRoll = Rotation.Rotator().Roll;
	}

	void HandleMovementInput(const FInputActionValue& Value);
	void HandleRotationInput(const FInputActionValue& Value);
};
