#pragma once

class FPerlinNoiseGenerator
{
	FVector SeedX;
	FVector SeedY;
	FVector SeedZ;
	FVector Position = FVector::ZeroVector;
	float Amplitude = 1.0f;
	float Scale = 1.0f;
	
public:
	explicit FPerlinNoiseGenerator(const float Amplitude, const float Scale);

	void Move(const float Delta);
	void Move(const FVector& Delta);
	FVector CurrentVector() const;
	FRotator CurrentRotation() const;
};
