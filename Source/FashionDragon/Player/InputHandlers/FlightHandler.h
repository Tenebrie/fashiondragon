#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"
#include "FashionDragon/Player/Generics/PlayerComponent.h"
#include "FlightHandler.generated.h"

class UDragonAnimInstance;
class FControlledWing;
class AMainCharacter;

UCLASS()
class FASHIONDRAGON_API UFlightHandler : public UPlayerComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly) bool IsInFlight = false;
	FControlledWing* LeftWing = nullptr;
	FControlledWing* RightWing = nullptr;

	UPROPERTY(BlueprintReadWrite) bool bIsFlapping = false;
	UPROPERTY(BlueprintReadWrite) bool bIsFoldingWings = false;
	UPROPERTY(BlueprintReadWrite) float WingAreaFactor = 1.0f;

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartFlight();
	void CancelFlight();
	bool IsFlying() const { return IsInFlight; }
	bool IsFlapping() const { return bIsFlapping; }
	bool IsFoldingWings() const { return bIsFoldingWings; }

	void HandleFlapForwardInput(const FInputActionValue& Value);
	void HandleFoldWingsInput(const FInputActionValue& Value);
};
