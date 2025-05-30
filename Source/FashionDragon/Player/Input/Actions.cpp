#include "Actions.h"

TArray<UInputAction*> UActions::InputActions;

FInputActionFactory UActions::Action(const EInputActionValueType Type)
{
	const int ActionIndex = ActionCount++;
	return [Type, ActionIndex]
	{
		while (InputActions.Num() <= ActionIndex)
			InputActions.Add(NewObject<UInputAction>());
		
		UInputAction* Action = InputActions[ActionIndex];
		Action->ValueType = Type;
		return Action;
	};
}

void UActions::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InputActions = TArray<UInputAction*>();
}
