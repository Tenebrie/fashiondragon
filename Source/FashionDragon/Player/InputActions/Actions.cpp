#include "Actions.h"

TArray<TStrongObjectPtr<UInputAction>> UActions::InputActions;

FInputActionFactory UActions::Action(const EInputActionValueType Type)
{
	const int ActionIndex = ActionCount++;
	return [Type, ActionIndex]
	{
		while (InputActions.Num() <= ActionIndex)
		{
			const auto NewAction = NewObject<UInputAction>();
			InputActions.Add(TStrongObjectPtr(NewAction));
		}

		UInputAction* Action = InputActions[ActionIndex].Get();
		Action->ValueType = Type;
		return Action;
	};
}

void UActions::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ActionCount = 0;
	InputActions = TArray<TStrongObjectPtr<UInputAction>>();
}
