#pragma once
#include <map>

class FProceduralPose;
class FDragonIdlePose;
class FDragonWalkPose;
class FDragonTrotPose;
class FDragonJumpPose;
class FDragonRandomSwayPose;
class FDragonMomentumPose;
class FDragonFootPlacementPose;
class AMainCharacter;

enum class EAnimationState
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
	FDragonRandomSwayPose* RandomSwayDriver;
	FDragonMomentumPose* MomentumDriver;
	FDragonFootPlacementPose* FootPlacementDriver;

	TArray<FProceduralPose*> PoseDrivers;
	std::map<EAnimationState, std::map<EAnimationState, std::function<void()>>> Transitions;

	FDragonAnimStateMachine(
		FDragonIdlePose* IdlePoseDriver,
		FDragonWalkPose* WalkPoseDriver,
		FDragonTrotPose* TrotPoseDriver,
		FDragonJumpPose* JumpPoseDriver,
		FDragonRandomSwayPose* RandomSwayDriver,
		FDragonMomentumPose* MomentumDriver,
		FDragonFootPlacementPose* FootPlacementDriver
	);

	float AnimationLockout = 0.0f;

	void InitTransitions();
	void Tick(float DeltaTime, const AMainCharacter* OwningActor);
	
	EAnimationState AnimationState = EAnimationState::Idle;
	void SetState(EAnimationState);

	void BlendDrivers(const float DeltaTime) const;
};
