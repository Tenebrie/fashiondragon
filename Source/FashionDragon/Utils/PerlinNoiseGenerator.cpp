#include  "PerlinNoiseGenerator.h"

FPerlinNoiseGenerator::FPerlinNoiseGenerator(const float Amplitude, const float Scale): Amplitude(Amplitude), Scale(Scale)
{
	const auto BaseSeed = FVector(
		FMath::FRand() * 256.f,
		FMath::FRand() * 256.f,
		FMath::FRand() * 256.f
	);
	SeedX = BaseSeed;
	SeedY = BaseSeed * FVector(0.37,1.23,2.17);
	SeedZ = BaseSeed * FVector(0.94,2.12,1.88);
}

void FPerlinNoiseGenerator::Move(const float Delta)
{
	Position += FVector(Delta, Delta, Delta) * Scale;
}

void FPerlinNoiseGenerator::Move(const FVector& Delta)
{
	Position += Delta * Scale;
}

FVector FPerlinNoiseGenerator::CurrentVector() const
{
	return FVector(
		FMath::PerlinNoise3D(Position + SeedX),
		FMath::PerlinNoise3D(Position + SeedY),
		FMath::PerlinNoise3D(Position + SeedZ)
	) * Amplitude;
}

FRotator FPerlinNoiseGenerator::CurrentRotation() const
{
	return FRotator(
		FMath::PerlinNoise3D(Position + SeedX) * Amplitude,
		FMath::PerlinNoise3D(Position + SeedY) * Amplitude,
		FMath::PerlinNoise3D(Position + SeedZ) * Amplitude
	);
}
