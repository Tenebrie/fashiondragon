#pragma once

class FControlledBone
{
public:
	FControlledBone() = default;
	FControlledBone(const FControlledBone& Other) = default;

	bool CanProduceEvents = false;
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

	virtual void Tick(const float DeltaTime) {}

	virtual ~FControlledBone() = default;
};
