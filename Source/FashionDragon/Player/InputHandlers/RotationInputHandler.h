#pragma once
#include "BaseInputHandler.h"
#include "RotationInputHandler.generated.h"

struct FInputActionValue;

UCLASS()
class URotationInputHandler final : public UBaseInputHandler
{
	GENERATED_BODY()

	float HorizontalSensitivity = 1.0f;
	float VerticalSensitivity = 1.0f;

	float CurrentPitch = 0.0f;
	float CurrentYaw = 0.0f;
	float CurrentRoll = 0.0f;
	
public:
	FRotator GetRotation() const { return FRotator(CurrentPitch, CurrentYaw, CurrentRoll); }
	void ResetRotation(const FQuat& Rotation)
	{
		CurrentPitch = Rotation.Rotator().Pitch;
		CurrentYaw = Rotation.Rotator().Yaw;
		CurrentRoll = Rotation.Rotator().Roll;
	}
	
	void HandleInput(const FInputActionValue& Value);
};
