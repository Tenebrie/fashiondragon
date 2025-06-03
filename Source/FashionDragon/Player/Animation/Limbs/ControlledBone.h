#pragma once
#include "FashionDragon/Player/Animation/Enums/DriverLayer.h"

class FControlledBone
{
public:
	FControlledBone() = default;
	FControlledBone(const FControlledBone& Other) = default;

	bool CanProduceEvents = false;
	EDriverLayer Layer = EDriverLayer::Unknown;
	FName GroupName = NAME_None;
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

	virtual void Tick(const float DeltaTime) {}

	virtual ~FControlledBone() = default;
};
