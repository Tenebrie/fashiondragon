#pragma once

class UDragonAnimInstance;

const enum ELegWalkingState
{
	Relaxed = 0,
	Raised = 1,
	Stepping = 3,
	Planted = 4,
	Inertia = 5,
};

class FControlledLeg
{
public:
	FControlledLeg(UDragonAnimInstance* AnimInstance, const int Idx);

private:
	UDragonAnimInstance* AnimInstance;
	int Idx;

public:
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
};
