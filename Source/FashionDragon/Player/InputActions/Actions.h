#pragma once
#include <functional>

#include "InputAction.h"

#include "Actions.generated.h"

using FInputActionFactory = std::function<UInputAction*()>;

UCLASS(BlueprintType)
class FASHIONDRAGON_API UActions : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	inline static int ActionCount = 0;
	static TArray<TStrongObjectPtr<UInputAction>> InputActions;
	static FInputActionFactory Action(EInputActionValueType Type);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	inline static const FInputActionFactory CameraMove = Action(EInputActionValueType::Axis2D);
	inline static const FInputActionFactory HoldAimDownSights = Action(EInputActionValueType::Boolean);
	
	inline static const FInputActionFactory GroundMovement = Action(EInputActionValueType::Axis2D);
	inline static const FInputActionFactory Jump = Action(EInputActionValueType::Boolean);
	inline static const FInputActionFactory Sprint = Action(EInputActionValueType::Boolean);
	inline static const FInputActionFactory TogglePreferredMovement = Action(EInputActionValueType::Boolean);
	inline static const FInputActionFactory CastASpell = Action(EInputActionValueType::Boolean);
	inline static const FInputActionFactory Interact = Action(EInputActionValueType::Boolean);

	inline static const FInputActionFactory CancelFlight = Action(EInputActionValueType::Boolean);

	inline static const FInputActionFactory ShowAnimDebug = Action(EInputActionValueType::Boolean);
	inline static const FInputActionFactory DebugButtonNum2 = Action(EInputActionValueType::Boolean);
	inline static const FInputActionFactory DebugButtonNum3 = Action(EInputActionValueType::Boolean);
};
