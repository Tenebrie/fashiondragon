#pragma once
#include "BaseInputHandler.h"
#include "RotationInputHandler.generated.h"

struct FInputActionValue;

UCLASS()
class URotationInputHandler final : public UBaseInputHandler
{
	GENERATED_BODY()

	float HorizontalSensitivity = 0.5f;
	float VerticalSensitivity = 1.0f;
	
	FQuat CurrentRotation = FQuat::Identity;
	
public:
	FQuat GetRotation() const { return CurrentRotation; }
	
	void HandleInput(const FInputActionValue& Value);
};
