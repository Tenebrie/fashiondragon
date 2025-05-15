#pragma once
#include <map>

#include "Poses/DragonFootPlacementPose.h"
#include "Poses/DragonIdlePose.h"
#include "Poses/DragonJumpPose.h"
#include "Poses/DragonTrotPose.h"
#include "Poses/DragonWalkPose.h"

class AMainCharacter;

const enum EAnimationState
{
	Idle,
	Walking,
	Running,
	Jumping,
};

class FDragonAnimStateMachine
{
public:
	FDragonIdlePose* IdlePoseDriver;
	FDragonWalkPose* WalkPoseDriver;
	FDragonTrotPose* TrotPoseDriver;
	FDragonJumpPose* JumpPoseDriver;
	FDragonFootPlacementPose* FootPlacementDriver;

	TArray<FProceduralPose*> PoseDrivers;
	std::map<EAnimationState, std::map<EAnimationState, std::function<void()>>> Transitions;

	FDragonAnimStateMachine(
		FDragonIdlePose* IdlePoseDriver,
		FDragonWalkPose* WalkPoseDriver,
		FDragonTrotPose* TrotPoseDriver,
		FDragonJumpPose* JumpPoseDriver,
		FDragonFootPlacementPose* FootPlacementDriver
	):
		IdlePoseDriver(IdlePoseDriver),
		WalkPoseDriver(WalkPoseDriver),
		TrotPoseDriver(TrotPoseDriver),
		JumpPoseDriver(JumpPoseDriver),
		FootPlacementDriver(FootPlacementDriver)
	{
		PoseDrivers = {
			IdlePoseDriver,
			WalkPoseDriver,
			TrotPoseDriver,
			JumpPoseDriver,
			FootPlacementDriver,
		};
		InitTransitions();
	}

	float AnimationLockout = 0.0f;

	void InitTransitions();
	void Tick(float DeltaTime, const AMainCharacter* OwningActor);
	
	EAnimationState AnimationState = Idle;
	void SetState(EAnimationState);

	void BlendDrivers(const float DeltaTime) const;
};
