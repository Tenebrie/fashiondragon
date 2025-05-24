#include "FlightController.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"

UFlightController::UFlightController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFlightController::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

AMainCharacter* UFlightController::GetCharacter() const
{
	return Cast<AMainCharacter>(GetOwner());
}
