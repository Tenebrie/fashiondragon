#pragma once
#include "FashionDragon/Player/Animation/Enums/DriverLayer.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

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
	FPoseWingEffector State = FPoseWingEffector();

	virtual void Tick(const float DeltaTime) {}

	virtual ~FControlledWing() = default;
};
