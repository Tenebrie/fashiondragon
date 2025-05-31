#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseInputHandler.generated.h"

class AMainCharacter;
class UDragonAnimInstance;
class UCharacterMovementComponent;

UCLASS()
class FASHIONDRAGON_API UBaseInputHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseInputHandler() { PrimaryComponentTick.bCanEverTick = true; }

	AMainCharacter* GetCharacter() const;
	UDragonAnimInstance* GetAnimInstance() const;
	UCharacterMovementComponent* GetCharacterMovement() const;
};
