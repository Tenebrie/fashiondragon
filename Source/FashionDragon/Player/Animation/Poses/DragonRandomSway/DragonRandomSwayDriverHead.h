#pragma once
#include "FashionDragon/Player/Animation/Abstract/ProceduralBoneDriver.h"
#include "FashionDragon/Utils/PerlinNoiseGenerator.h"

enum class EHeadLookState
{
	Relaxed,
	LookingAround,
	LookingAtTarget,
};

class FDragonRandomSwayDriverHead final : public FProceduralBoneDriver
{
	float TransitionPosition = 0.0f;
	float TransitionDuration = 0.5f;
	FRotator TransitioningFrom = FRotator::ZeroRotator;
	
	float Cooldown = 2.0f;
	int LookingAroundStatesLeft = 0;
	EHeadLookState HeadLookState = EHeadLookState::Relaxed;
	
public:
	FDragonRandomSwayDriverHead(UDragonAnimInstance* AnimInstance, FControlledBone* Bone)
		: FProceduralBoneDriver(AnimInstance, Bone)
	{
		DesiredForce = 1.0f;
	}

	virtual void Tick(float DeltaTime) override;
	virtual FPoseEffector ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context) override;
};
