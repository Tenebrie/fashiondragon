#include "DragonIdleBodyDriver.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

void FDragonIdleBodyDriver::Tick(const float DeltaTime)
{
	DesiredRotation = FMath::RInterpTo(Bone->Rotation, FRotator(0.0f, 0.0f, -10.0f), DeltaTime, 1.8f);
}
