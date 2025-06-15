#include "DragonFlightDriverLeg.h"

void FDragonFlightDriverLeg::Tick(const float DeltaTime)
{
	FProceduralLegSteppingDriver::Tick(DeltaTime);

	DesiredPosition = FVector(-450, 0.0f,  250.0f);
	DesiredRotation = FRotator(-120, 0, 0);
}
