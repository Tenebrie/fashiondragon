#pragma once

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/Generics/PlayerComponent.h"
#include "RotationInputHandler.generated.h"

struct FInputActionValue;

UCLASS(BlueprintType)
class FASHIONDRAGON_API URotationInputHandler : public UPlayerComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite) FVector2D UserInput = FVector2D::ZeroVector;
	UPROPERTY(BlueprintReadWrite) FVector2D WorldUserInput = FVector2D::ZeroVector;
	UPROPERTY(BlueprintReadWrite) FRotator MovementRotation = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadWrite) FVector SmoothMovementRotation = FVector::ZeroVector;

	UPROPERTY() float HorizontalSensitivity = 1.0f;
	UPROPERTY() float VerticalSensitivity = 1.0f;

	UPROPERTY(BlueprintReadWrite) float CurrentPitch = 0.0f;
	UPROPERTY(BlueprintReadWrite) float CurrentYaw = 0.0f;
	UPROPERTY(BlueprintReadWrite) float CurrentRoll = 0.0f;
	
	UFUNCTION(BlueprintCallable) FRotator GetInputForwardWorldRotation() const { return MovementRotation; }
	UFUNCTION(BlueprintCallable) FVector GetSmoothInputForwardWorldRotation() const { return SmoothMovementRotation; }
	UFUNCTION(BlueprintCallable) FRotator GetCameraWorldRotation() const { return FRotator(CurrentPitch, CurrentYaw, CurrentRoll); }
	UFUNCTION(BlueprintCallable) void ResetRotation(const FQuat& Rotation)
	{
		const FVector FlatInputVector = Rotation.Vector().GetSafeNormal2D();
		UserInput = FVector2D(FlatInputVector.X, FlatInputVector.Y);
		MovementRotation = Rotation.Rotator();
		CurrentPitch = Rotation.Rotator().Pitch;
		CurrentYaw = Rotation.Rotator().Yaw;
		CurrentRoll = Rotation.Rotator().Roll;
	}

	void HandleMovementInput(const FInputActionValue& Value);
	void HandleRotationInput(const FInputActionValue& Value);
};
