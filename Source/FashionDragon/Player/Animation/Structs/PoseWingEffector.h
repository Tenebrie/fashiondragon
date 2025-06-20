#pragma once
#include "ArticulatedValue.h"
#include "PoseWingEffector.generated.h"

USTRUCT(BlueprintType)
struct FPoseWingEffector
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly) float FlapAngle = 0.0f;
	UPROPERTY(BlueprintReadOnly) float TiltAngle = 0.0f;
	// Fold state when active. Will artifact on extreme values.
	UPROPERTY(BlueprintReadOnly) float FlightFoldState = 0.0f;
	// Fold state at rest. Overrides other properties.
	UPROPERTY(BlueprintReadOnly) float RestFoldState = 0.0f;

	FPoseWingEffector() {}
	FPoseWingEffector(FPoseWingEffector const&) = default;
	FPoseWingEffector(const float FlapAngle, const float TiltAngle, const float FlightFoldState, const float RestFoldState):
		FlapAngle(FlapAngle), TiltAngle(TiltAngle), FlightFoldState(FlightFoldState), RestFoldState(RestFoldState) {}

	FPoseWingEffector AddFlapAngle(const float Delta) const
	{
		FPoseWingEffector Copy = *this;
		Copy.FlapAngle += Delta;
		return Copy;
	}
	FPoseWingEffector AddTiltAngle(const float Delta) const
	{
		FPoseWingEffector Copy = *this;
		Copy.TiltAngle += Delta;
		return Copy;
	}
	FPoseWingEffector AddFlightFoldState(const float Delta) const
	{
		FPoseWingEffector Copy = *this;
		Copy.FlightFoldState += Delta;
		return Copy;
	}
	FPoseWingEffector AddRestFoldState(const float Delta) const
	{
		FPoseWingEffector Copy = *this;
		Copy.RestFoldState += Delta;
		return Copy;
	}

	FPoseWingEffector SetFlapAngle(const float NewValue) const
	{
		FPoseWingEffector Copy = *this;
		Copy.FlapAngle = NewValue;
		return Copy;
	}
	FPoseWingEffector SetTiltAngle(const float NewValue) const
	{
		FPoseWingEffector Copy = *this;
		Copy.TiltAngle = NewValue;
		return Copy;
	}
	FPoseWingEffector SetFlightFoldState(const float NewValue) const
	{
		FPoseWingEffector Copy = *this;
		Copy.FlightFoldState = NewValue;
		return Copy;
	}
	FPoseWingEffector SetRestFoldState(const float NewValue) const
	{
		FPoseWingEffector Copy = *this;
		Copy.RestFoldState = NewValue;
		return Copy;
	}

	FPoseWingEffector InterpTo(const FPoseWingEffector& Other, const float DeltaTime, const float InterpSpeed) const
	{
		return FPoseWingEffector(
			FMath::FInterpTo(FlapAngle, Other.FlapAngle, DeltaTime, InterpSpeed),
			FMath::FInterpTo(TiltAngle, Other.TiltAngle, DeltaTime, InterpSpeed),
			FMath::FInterpTo(FlightFoldState, Other.FlightFoldState, DeltaTime, InterpSpeed),
			FMath::FInterpTo(RestFoldState, Other.RestFoldState, DeltaTime, InterpSpeed)
		);
	}
	
	FPoseWingEffector operator+(const FPoseWingEffector& Other) const
	{
		return FPoseWingEffector(
			FlapAngle + Other.FlapAngle,
			TiltAngle + Other.TiltAngle,
			FlightFoldState + Other.FlightFoldState,
			RestFoldState + Other.RestFoldState
		);
	}
	FPoseWingEffector operator-(const FPoseWingEffector& Other) const
	{
		return FPoseWingEffector(
			FlapAngle - Other.FlapAngle,
			TiltAngle - Other.TiltAngle,
			FlightFoldState - Other.FlightFoldState,
			RestFoldState - Other.RestFoldState
		);
	}
	FPoseWingEffector operator*(const float Multiplier) const
	{
		return FPoseWingEffector(
			FlapAngle * Multiplier,
			TiltAngle * Multiplier,
			FlightFoldState * Multiplier,
			RestFoldState * Multiplier
		);
	}
	FPoseWingEffector& operator+=(const FPoseWingEffector& Other)
	{
		FlapAngle += Other.FlapAngle;
		TiltAngle += Other.TiltAngle;
		FlightFoldState += Other.FlightFoldState;
		RestFoldState += Other.RestFoldState;
		return *this;
	}
};
