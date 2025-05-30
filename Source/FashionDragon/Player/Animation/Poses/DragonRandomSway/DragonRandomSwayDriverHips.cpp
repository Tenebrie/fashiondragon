#include "DragonRandomSwayDriverHips.h"

#include "FashionDragon/Utils/Utils.h"

void FDragonRandomSwayDriverHips::Tick(const float DeltaTime)
{
	FProceduralBoneDriver::Tick(DeltaTime);
	RotationSwayNoise.Move(DeltaTime);
}

FPoseEffector FDragonRandomSwayDriverHips::ToEffector(const FPoseEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	auto NoiseRotation = RotationSwayNoise.CurrentRotation();
	NoiseRotation.Roll = 0.0f;
	NoiseRotation.Pitch *= 0.25f;
	return BaseEffector.SetRotation(NoiseRotation);
}
