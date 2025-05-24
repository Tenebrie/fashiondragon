#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlightController.generated.h"

UCLASS()
class FASHIONDRAGON_API UFlightController : public UActorComponent
{
	GENERATED_BODY()
public:
	UFlightController();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
