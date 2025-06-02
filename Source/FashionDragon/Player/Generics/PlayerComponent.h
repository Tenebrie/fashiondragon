#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerComponent.generated.h"

class AMainCharacter;
class UDragonAnimInstance;
class UCharacterMovementComponent;

UCLASS()
class FASHIONDRAGON_API UPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerComponent() { PrimaryComponentTick.bCanEverTick = true; }

	AMainCharacter* GetCharacter() const;
	UDragonAnimInstance* GetAnimInstance() const;
	UCharacterMovementComponent* GetCharacterMovement() const;
};
