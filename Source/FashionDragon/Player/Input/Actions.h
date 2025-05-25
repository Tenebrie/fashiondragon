#pragma once
#include "InputAction.h"

#include "Actions.generated.h"

UCLASS()
class UActions : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	static const UInputAction* Action(EInputActionValueType Type);

public:
	inline static const UInputAction* GroundMovement = Action(EInputActionValueType::Axis2D);
	inline static const UInputAction* CameraMove = Action(EInputActionValueType::Axis2D);
	inline static const UInputAction* Jump = Action(EInputActionValueType::Boolean);
	inline static const UInputAction* Sprint = Action(EInputActionValueType::Boolean);
	inline static const UInputAction* TogglePreferredMovement = Action(EInputActionValueType::Boolean);
	inline static const UInputAction* CastASpell = Action(EInputActionValueType::Boolean);

	inline static const UInputAction* FlightCamera = Action(EInputActionValueType::Axis2D);
};
