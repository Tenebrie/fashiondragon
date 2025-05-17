#pragma once
#include "BasePlayerController.h"

#include "DefaultPlayerController.generated.h"

UCLASS()
class FASHIONDRAGON_API ADefaultPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

	virtual void SetupInputComponent() override;
	void SetupInputMappings();
	
public:
	UPROPERTY() UInputMappingContext* DragonGroundInputContext;
	UPROPERTY() UInputMappingContext* DragonFlyingInputContext;
};
