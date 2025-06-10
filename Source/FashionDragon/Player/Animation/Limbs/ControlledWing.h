#pragma once
#include "FashionDragon/Player/Animation/Enums/DriverLayer.h"

class UDragonAnimInstance;

class FControlledWing final
{
	UDragonAnimInstance* AnimInstance;
	int Idx;
	
public:
	FControlledWing(UDragonAnimInstance* AnimInstance, const int Idx)
		: AnimInstance(AnimInstance), Idx(Idx)
	{
		if (Idx == 1)
		{
			MirrorModifier = -1.0f;
		}
	}

	bool CanProduceEvents = false;
	FName GroupName = NAME_None;
	EDriverLayer Layer = EDriverLayer::Unknown;
	FVector Position = FVector::ZeroVector;
	
	float MirrorModifier = 1.0f;
	// Flap state [-1.0f, 1.0f]
	float Flap = 0.0f;
	// 0: Wing folded, 1: Wing fully extended
	float Openness = 1.0f;

	FName GetBoneName(int BoneIdx) const;

	virtual void Tick(const float DeltaTime) {}

	virtual ~FControlledWing() = default;
};
