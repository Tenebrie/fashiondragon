#include "BasePlayerController.h"

#include "Actions.h"
#include "EnhancedInputComponent.h"
#include "FashionDragon/DebugTools/QuickDebug.h"

FKeyMappingBuilder& FKeyMappingBuilder::AddTrigger(UInputTrigger* Trigger)
{
	Mapping->Triggers.Add(Trigger);
	return *this;
}

FKeyMappingBuilder& FKeyMappingBuilder::OnKeyDown()
{
	return AddTrigger(NewObject<UInputTriggerDown>(Context));
}

FKeyMappingBuilder& FKeyMappingBuilder::OnKeyUp()
{
	return AddTrigger(NewObject<UInputTriggerReleased>(Context));
}

FKeyMappingBuilder& FKeyMappingBuilder::AddModifier(UInputModifier* Modifier)
{
	Mapping->Modifiers.Add(Modifier);
	return *this;
}

FKeyMappingBuilder& FKeyMappingBuilder::Value(const float Value)
{
	const auto Modifier = NewObject<UInputModifierScalar>(Context);
	Modifier->Scalar = FVector(Value, Value, Value);
	return AddModifier(Modifier);
}

FKeyMappingBuilder& FKeyMappingBuilder::AsTrue()
{
	return Value(1.0f);
}

FKeyMappingBuilder& FKeyMappingBuilder::AsFalse()
{
	return Value(0.0f);
}

FKeyMappingBuilder& FKeyMappingBuilder::Negate()
{
	return AddModifier(NewObject<UInputModifierNegate>(Context));
}

FKeyMappingBuilder& FKeyMappingBuilder::Swizzle()
{
	return AddModifier(NewObject<UInputModifierSwizzleAxis>(Context));
}

FKeyMappingBuilder& FKeyMappingBuilder::DeadZone(const float Value)
{
	const auto Modifier = NewObject<UInputModifierDeadZone>(Context);
	Modifier->Type = EDeadZoneType::Radial;
	Modifier->LowerThreshold = Value;
	Modifier->UpperThreshold = 1.0f;
	return AddModifier(Modifier);
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	if (!Input) return;

	Input->BindAction(UActions::ShowAnimDebug(), ETriggerEvent::Started, this, &ABasePlayerController::OnShowAnimDebug);
	Input->BindAction(UActions::DebugButtonNum2(), ETriggerEvent::Started, this, &ABasePlayerController::OnDebugButton2);
	Input->BindAction(UActions::DebugButtonNum3(), ETriggerEvent::Started, this, &ABasePlayerController::OnDebugButton3);
}

UInputMappingContext* ABasePlayerController::MakeInputContext()
{
	return NewObject<UInputMappingContext>(this);
}

FKeyMappingBuilder ABasePlayerController::AddMapping(UInputMappingContext* Context, const UInputAction* Value, const FKey& Key)
{
	FEnhancedActionKeyMapping* Mapping = &Context->MapKey(Value, Key);
	return FKeyMappingBuilder(Context, Mapping);
}
