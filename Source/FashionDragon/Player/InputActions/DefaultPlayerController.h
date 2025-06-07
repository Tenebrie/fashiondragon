#pragma once
#include "BasePlayerController.h"

#include "DefaultPlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;

enum class EControlMode : uint8
{
	None,
	Ground,
	Flying,
};

UCLASS()
class FASHIONDRAGON_API ADefaultPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

	virtual void SetupInputComponent() override;
	void SetupInputMappings();

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem;

	EControlMode ControlMode = EControlMode::None;
	
public:
	void SetControlMode(EControlMode Mode) const;

	UPROPERTY() UInputMappingContext* CommonInputContext;
	UPROPERTY() UInputMappingContext* DragonGroundInputContext;
	UPROPERTY() UInputMappingContext* DragonFlyingInputContext;
	UPROPERTY() UInputMappingContext* DebugInputContext;
};
