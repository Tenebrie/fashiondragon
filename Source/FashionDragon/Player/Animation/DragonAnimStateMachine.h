#pragma once
#include <functional>
#include <map>

class FProceduralPose;
class FDragonNullPose;
class FDragonIdlePose;
class FDragonWalkPose;
class FDragonTrotPose;
class FDragonSprintPose;
class FDragonJumpPose;
class FDragonFlightPose;
class FDragonRandomSwayPose;
class FDragonMomentumPose;
class FCapsuleCompensationPose;
class FDragonFootPlacementPose;
class AMainCharacter;

enum class EAnimationState
{
	Idle,
	Walking,
	Trotting,
	Sprinting,
	Jumping,
	Flight,
};

class FDragonAnimStateMachine
{
public:
	FDragonNullPose* NullPoseDriver;
	FDragonIdlePose* IdlePoseDriver;
	FDragonWalkPose* WalkPoseDriver;
	FDragonTrotPose* TrotPoseDriver;
	FDragonSprintPose* SprintPoseDriver;
	FDragonJumpPose* JumpPoseDriver;
	FDragonFlightPose* FlightPoseDriver;
	FDragonRandomSwayPose* RandomSwayDriver;
	FDragonMomentumPose* MomentumDriver;
	FCapsuleCompensationPose* CapsuleCompensationDriver;
	FDragonFootPlacementPose* FootPlacementDriver;

	TArray<FProceduralPose*> PoseDrivers;
	std::map<EAnimationState, std::map<EAnimationState, std::function<void()>>> Transitions;

	FDragonAnimStateMachine(
		FDragonNullPose* NullPoseDriver,
		FDragonIdlePose* IdlePoseDriver,
		FDragonWalkPose* WalkPoseDriver,
		FDragonTrotPose* TrotPoseDriver,
		FDragonSprintPose* SprintPoseDriver,
		FDragonJumpPose* JumpPoseDriver,
		FDragonFlightPose* FlightPoseDriver,
		FDragonRandomSwayPose* RandomSwayDriver,
		FDragonMomentumPose* MomentumDriver,
		FCapsuleCompensationPose* CapsuleCompensationDriver,
		FDragonFootPlacementPose* FootPlacementDriver
	);

	float AnimationLockout = 0.0f;

	void NativeBeginPlay();
	void InitTransitions();
	void Tick(float DeltaTime, const AMainCharacter* OwningActor);
	
	EAnimationState AnimationState = EAnimationState::Idle;
	void SetState(EAnimationState);

	void BlendDrivers(const float DeltaTime) const;
};
