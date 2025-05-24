#include "FlightController.h"

#include "FashionDragon/DebugTools/QuickDebug.h"

UFlightController::UFlightController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlightController::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Debug::Print("Test");
}
