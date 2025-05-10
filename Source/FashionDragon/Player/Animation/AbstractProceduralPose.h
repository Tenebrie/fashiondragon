#pragma once
#include "AbstractProceduralLegDriver.h"

struct FPoseEffector
{
	mutable float Weight = 0.0f;
	mutable FVector Position = FVector(0.0f, 0.0f, 0.0f);
	mutable FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
};

class FAbstractProceduralPose
{
protected:
	UDragonAnimInstance* AnimInstance;

	TArray<FAbstractProceduralLegDriver*> LegDrivers;
	
public:
	virtual ~FAbstractProceduralPose() = default;

	explicit FAbstractProceduralPose(UDragonAnimInstance* AnimInstance);

	float BlendAlpha = 0.0f;
	virtual void Tick(float DeltaTime);
	virtual void ResetState() {};
	FPoseEffector ToLegEffector(const FControlledLeg* Leg) const; 
};
