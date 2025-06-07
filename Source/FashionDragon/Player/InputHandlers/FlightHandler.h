#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FashionDragon/Player/Generics/PlayerComponent.h"
#include "FlightHandler.generated.h"

class UDragonAnimInstance;
class FControlledWing;
class AMainCharacter;

UCLASS()
class FASHIONDRAGON_API UFlightHandler : public UPlayerComponent
{
	GENERATED_BODY()

	bool IsInFlight = false;
	FControlledWing* LeftWing = nullptr;
	FControlledWing* RightWing = nullptr;

	float LastFrameLeftFlap = 0.0f;
	float LastFrameRightFlap = 0.0f;
	float LastFrameLeftOpenness = 0.0f;
	float LastFrameRightOpenness = 0.0f;

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartFlight();
	void CancelFlight();
	bool IsFlying() const { return IsInFlight; }
};
