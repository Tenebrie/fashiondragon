#pragma once
#include "InputMappingContext.h"

#include "BasePlayerController.generated.h"

class FKeyMappingBuilder
{
	UInputMappingContext* Context;
	FEnhancedActionKeyMapping* Mapping;

public:
	explicit FKeyMappingBuilder(UInputMappingContext* Context, FEnhancedActionKeyMapping* Mapping) : Context(Context), Mapping(Mapping) {}

	FKeyMappingBuilder& AddTrigger(UInputTrigger* Trigger);
	FKeyMappingBuilder& OnKeyDown();
	FKeyMappingBuilder& OnKeyUp();
	
	FKeyMappingBuilder& AddModifier(UInputModifier* Modifier);
	FKeyMappingBuilder& Value(float Value);
	FKeyMappingBuilder& AsTrue();
	FKeyMappingBuilder& AsFalse();
	FKeyMappingBuilder& Negate();
	FKeyMappingBuilder& Swizzle();
	FKeyMappingBuilder& DeadZone(float Value);
};

UCLASS()
class FASHIONDRAGON_API ABasePlayerController  : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UInputMappingContext* MakeInputContext();
	static FKeyMappingBuilder AddMapping(UInputMappingContext* Context, const UInputAction* Value, const FKey& Key);
};
