#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlightController.generated.h"

class UDragonAnimInstance;
class FControlledWing;
class AMainCharacter;

UCLASS()
class FASHIONDRAGON_API UFlightController : public UActorComponent
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
	UFlightController();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartFlight();
	bool IsFlying() const { return IsInFlight; }

	AMainCharacter* GetCharacter() const;
	UDragonAnimInstance* GetAnimInstance() const;
};
