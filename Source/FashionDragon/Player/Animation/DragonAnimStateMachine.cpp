#include "DragonAnimStateMachine.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Utils/Utils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Poses/DragonFootPlacement/DragonFootPlacementPose.h"
#include "Poses/DragonIdle/DragonIdlePose.h"
#include "Poses/DragonJump/DragonJumpPose.h"
#include "Poses/DragonRandomSway/DragonRandomSwayPose.h"
#include "Poses/DragonTrot/DragonTrotPose.h"
#include "Poses/DragonWalk/DragonWalkPose.h"
#include "Poses/DragonSprint/DragonSprintPose.h"
#include "Poses/DragonMomentum/DragonMomentumPose.h"

FDragonAnimStateMachine::FDragonAnimStateMachine(
	FDragonIdlePose* IdlePoseDriver,
	FDragonWalkPose* WalkPoseDriver,
	FDragonTrotPose* TrotPoseDriver,
	FDragonSprintPose* SprintPoseDriver,
	FDragonJumpPose* JumpPoseDriver,
	FDragonRandomSwayPose* RandomSwayDriver,
	FDragonMomentumPose* MomentumDriver,
	FDragonFootPlacementPose* FootPlacementDriver
):
	IdlePoseDriver(IdlePoseDriver),
	WalkPoseDriver(WalkPoseDriver),
	TrotPoseDriver(TrotPoseDriver),
	SprintPoseDriver(SprintPoseDriver),
	JumpPoseDriver(JumpPoseDriver),
	RandomSwayDriver(RandomSwayDriver),
	MomentumDriver(MomentumDriver),
	FootPlacementDriver(FootPlacementDriver)
{
	PoseDrivers = {
		IdlePoseDriver,
		WalkPoseDriver,
		TrotPoseDriver,
		SprintPoseDriver,
		JumpPoseDriver,
		RandomSwayDriver,
		MomentumDriver,
		FootPlacementDriver,
	};
	InitTransitions();
}

void FDragonAnimStateMachine::NativeBeginPlay()
{
	IdlePoseDriver->ResetState();
	for (FProceduralPose* PoseDriver : PoseDrivers)
		PoseDriver->NativeBeginPlay();
}

void FDragonAnimStateMachine::InitTransitions()
{
	Transitions = {
		{ EAnimationState::Idle, {
			{ EAnimationState::Walking, [this] { WalkPoseDriver->ResetState(); }},
			{ EAnimationState::Trotting, [this] { TrotPoseDriver->ResetState(); }},
			{ EAnimationState::Sprinting, [this] { SprintPoseDriver->ResetState(); }},
			{ EAnimationState::Jumping, [this] { JumpPoseDriver->ResetState(); }},
		}},
		{ EAnimationState::Jumping, {
			{ EAnimationState::Idle, [this] { IdlePoseDriver->ResetState(); }},
			{ EAnimationState::Walking, [this] { WalkPoseDriver->ResetState(); }},
			{ EAnimationState::Trotting, [this] { TrotPoseDriver->ResetState(); }},
			{ EAnimationState::Sprinting, [this] { SprintPoseDriver->ResetState(); }},
			{ EAnimationState::Jumping, [this] { JumpPoseDriver->ResetState(); }},
		}},
		{ EAnimationState::Walking, {
			{ EAnimationState::Idle, [this] { IdlePoseDriver->SyncStateFrom(WalkPoseDriver); }},
			{ EAnimationState::Trotting, [this] { TrotPoseDriver->SyncStateFrom(WalkPoseDriver); }},
			{ EAnimationState::Sprinting, [this] { SprintPoseDriver->SyncStateFrom(WalkPoseDriver); }},
			{ EAnimationState::Jumping, [this] { JumpPoseDriver->ResetState(); }},
		}},
		{ EAnimationState::Trotting, {
			{ EAnimationState::Idle, [this] { IdlePoseDriver->SyncStateFrom(TrotPoseDriver); }},
			{ EAnimationState::Walking, [this] { WalkPoseDriver->SyncStateFrom(TrotPoseDriver); } },
			{ EAnimationState::Sprinting, [this] { SprintPoseDriver->SyncStateFrom(TrotPoseDriver); } },
			{ EAnimationState::Jumping, [this] { JumpPoseDriver->ResetState(); }},
		}},
		{ EAnimationState::Sprinting, {
			{ EAnimationState::Idle, [this] { IdlePoseDriver->SyncStateFrom(SprintPoseDriver); }},
			{ EAnimationState::Walking, [this] { WalkPoseDriver->SyncStateFrom(SprintPoseDriver); } },
			{ EAnimationState::Trotting, [this] { TrotPoseDriver->SyncStateFrom(SprintPoseDriver); } },
			{ EAnimationState::Jumping, [this] { JumpPoseDriver->ResetState(); }},
		}}
	};
}

void FDragonAnimStateMachine::Tick(const float DeltaTime, const AMainCharacter* OwningActor)
{
	AnimationLockout -= DeltaTime;
	if (AnimationLockout < 0.0f)
		AnimationLockout = 0.0f;

	const auto MovementVector = FVector(
		OwningActor->GetLastMovementInputVector().X,
		OwningActor->GetLastMovementInputVector().Y,
		0.0f
	);
	// if (AnimationState == Jumping)
	// {
	// 	AnimationLockout = 0.5f;
	// }

	if (AnimationLockout <= 0.0f)
	{
		const bool CanSwitchWalkAnim = FUtils::IsOneOf(AnimationState,
			EAnimationState::Idle, EAnimationState::Walking, EAnimationState::Trotting, EAnimationState::Sprinting);
		
		if (AnimationState == EAnimationState::Jumping && !OwningActor->GetCharacterMovement()->IsFalling())
		{
			SetState(EAnimationState::Idle);
		}
		if (MovementVector.Size() == 0 && CanSwitchWalkAnim && AnimationState != EAnimationState::Idle)
		{
			SetState(EAnimationState::Idle);
		}
		if (MovementVector.Size() > 0 && CanSwitchWalkAnim && AnimationState != EAnimationState::Walking && OwningActor->MovementMode == ECharacterMovementMode::Walking)
		{
			SetState(EAnimationState::Walking);
		}
		if (MovementVector.Size() > 0 && CanSwitchWalkAnim && AnimationState != EAnimationState::Trotting && OwningActor->MovementMode == ECharacterMovementMode::Trotting)
		{
			SetState(EAnimationState::Trotting);
		}
		if (MovementVector.Size() > 0 && CanSwitchWalkAnim && AnimationState != EAnimationState::Sprinting && OwningActor->MovementMode == ECharacterMovementMode::Sprinting)
		{
			SetState(EAnimationState::Sprinting);
		}
		if (AnimationState != EAnimationState::Jumping && OwningActor->GetCharacterMovement()->IsFalling())
		{
			SetState(EAnimationState::Jumping);
			JumpPoseDriver->StartFalling();
		}
	}

	for (int i = 0; i < PoseDrivers.Num(); i++)
	{ 
		const auto PoseDriver = PoseDrivers[i];
		PoseDriver->Tick(DeltaTime);
	}

	BlendDrivers(DeltaTime);
}

/**
 * @brief Merge the drivers' current positions and apply them to controlled entities.
 */
void FDragonAnimStateMachine::BlendDrivers(const float DeltaTime) const
{
	FProceduralPose* DominantDriver = IdlePoseDriver;
	if (AnimationState == EAnimationState::Idle)
	{
		DominantDriver = IdlePoseDriver;
	}
	else if (AnimationState == EAnimationState::Walking)
	{
		DominantDriver = WalkPoseDriver;
	}
	else if (AnimationState == EAnimationState::Trotting)
	{
		DominantDriver = TrotPoseDriver;
	}
	else if (AnimationState == EAnimationState::Sprinting)
	{
		DominantDriver = SprintPoseDriver;
	}
	else if (AnimationState == EAnimationState::Jumping)
	{
		DominantDriver = JumpPoseDriver;
	}

	DominantDriver->AddBlendAlpha(DeltaTime * 100.0f);
	
	for (FProceduralPose* PoseDriver : PoseDrivers)
	{
		if (PoseDriver != DominantDriver)
		{
			PoseDriver->AddBlendAlpha(-DeltaTime * 100.0f);
		}
	}

	MomentumDriver->SetBlendAlpha(1.0f);
	RandomSwayDriver->SetBlendAlpha(1.0f);
	FootPlacementDriver->SetBlendAlpha(1.0f);
}

/**
 * @brief Switches the animation state in response to player's input.
 */
void FDragonAnimStateMachine::SetState(const EAnimationState NewState)
{
	if (Transitions.contains(AnimationState))
	{
		const auto TransitionsFrom = Transitions.at(AnimationState);
		if (TransitionsFrom.contains(NewState))
		{
			const auto OnTransition = TransitionsFrom.at(NewState);
			OnTransition();
		}
	}

	this->AnimationState = NewState;
}
