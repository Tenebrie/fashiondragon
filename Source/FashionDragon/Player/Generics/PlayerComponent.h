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

	UFUNCTION(BlueprintCallable) AMainCharacter* GetCharacter() const;
	UFUNCTION(BlueprintCallable) UDragonAnimInstance* GetAnimInstance() const;
	UFUNCTION(BlueprintCallable) UCharacterMovementComponent* GetCharacterMovement() const;
};
