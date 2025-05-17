#include "Actions.h"

const UInputAction* UActions::Action(const EInputActionValueType Type)
{
	UInputAction* Action = NewObject<UInputAction>();
	Action->ValueType = Type;
	return Action;
}
