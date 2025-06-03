#include "DragonRandomSwayDriverRoot.h"

#include "FashionDragon/DebugTools/QuickDebug.h"

void FDragonRandomSwayDriverRoot::SetPositionAmplitude(const float Amplitude, const float Scale)
{
	PositionSwayNoise = FPerlinNoiseGenerator(Amplitude, Scale);
}

void FDragonRandomSwayDriverRoot::SetRotationAmplitude(const float Amplitude, const float Scale)
{
	RotationSwayNoise = FPerlinNoiseGenerator(Amplitude, Scale);
}

void FDragonRandomSwayDriverRoot::Tick(const float DeltaTime)
{
	FProceduralBoneDriver::Tick(DeltaTime);
	PositionSwayNoise.Move(DeltaTime);
	RotationSwayNoise.Move(DeltaTime);
	BreathCycle += DeltaTime;
}

FPoseEffector FDragonRandomSwayDriverRoot::ToEffector(const FPoseEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	const auto BreathCyclePosition = FMath::Sin(BreathCycle * 0.5f * PI) * 0.5f + 0.5f;
	const FVector BreathCycleOffset = FVector(0.0f, 0.0f, BreathCyclePosition * 5.0f);
	
	const auto NoiseOffset = PositionSwayNoise.CurrentVector();
	const auto NoiseRotation = RotationSwayNoise.CurrentRotation();
	return BaseEffector.SetPosition(NoiseOffset).SetRotation(NoiseRotation).AddPosition(BreathCycleOffset);
}
