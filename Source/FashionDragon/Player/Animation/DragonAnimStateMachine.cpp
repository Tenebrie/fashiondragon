#include "DragonAnimStateMachine.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Poses/DragonFootPlacement/DragonFootPlacementPose.h"
#include "Poses/DragonIdle/DragonIdlePose.h"
#include "Poses/DragonJump/DragonJumpPose.h"
#include "Poses/DragonRandomSway/DragonRandomSwayPose.h"
#include "Poses/DragonTrot/DragonTrotPose.h"
#include "Poses/DragonWalk/DragonWalkPose.h"
#include "Poses/DragonMomentum/DragonMomentumPose.h"

FDragonAnimStateMachine::FDragonAnimStateMachine(
	FDragonIdlePose* IdlePoseDriver,
	FDragonWalkPose* WalkPoseDriver,
	FDragonTrotPose* TrotPoseDriver,
	FDragonJumpPose* JumpPoseDriver,
	FDragonRandomSwayPose* RandomSwayDriver,
	FDragonMomentumPose* MomentumDriver,
	FDragonFootPlacementPose* FootPlacementDriver
):
	IdlePoseDriver(IdlePoseDriver),
	WalkPoseDriver(WalkPoseDriver),
	TrotPoseDriver(TrotPoseDriver),
	JumpPoseDriver(JumpPoseDriver),
	RandomSwayDriver(RandomSwayDriver),
	MomentumDriver(MomentumDriver),
	FootPlacementDriver(FootPlacementDriver)
{
	PoseDrivers = {
		IdlePoseDriver,
		WalkPoseDriver,
		TrotPoseDriver,
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
			{ EAnimationState::Walking, [this]
			{
				WalkPoseDriver->ResetState();
				WalkPoseDriver->SetBlendAlpha(1.0f);
			}},
			{ EAnimationState::Running, [this]
			{
				TrotPoseDriver->ResetState();
				TrotPoseDriver->SetBlendAlpha(1.0f);
			}},
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->ResetState();
				JumpPoseDriver->SetBlendAlpha(1.0f);
			}},
		}},
		{ EAnimationState::Jumping, {
			{ EAnimationState::Idle, [this]
			{
				IdlePoseDriver->ResetState();
			}},
			{ EAnimationState::Walking, [this]
			{
				WalkPoseDriver->ResetState();
			}},
			{ EAnimationState::Running, [this]
			{
				TrotPoseDriver->ResetState();
			}},
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->ResetState();
			}},
		}},
		{ EAnimationState::Walking, {
			{ EAnimationState::Idle, [this] { IdlePoseDriver->SyncStateFrom(WalkPoseDriver); } },
			{ EAnimationState::Running, [this] { TrotPoseDriver->SyncStateFrom(WalkPoseDriver); } },
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->ResetState();
				JumpPoseDriver->SetBlendAlpha(1.0f);
			} },
		}},
		{ EAnimationState::Running, {
			{ EAnimationState::Walking, [this] { WalkPoseDriver->SyncStateFrom(TrotPoseDriver); } },
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->ResetState();
				JumpPoseDriver->SetBlendAlpha(1.0f);
			} },
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
		if (AnimationState == EAnimationState::Jumping && !OwningActor->GetCharacterMovement()->IsFalling())
		{
			SetState(EAnimationState::Idle);
		}
		if (MovementVector.Size() > 0 && !OwningActor->IsSprinting && (AnimationState == EAnimationState::Idle || AnimationState == EAnimationState::Running))
		{
			SetState(EAnimationState::Walking);
		}
		if (MovementVector.Size() > 0 && OwningActor->IsSprinting && (AnimationState == EAnimationState::Idle || AnimationState == EAnimationState::Walking))
		{
			SetState(EAnimationState::Running);
		}
		if (MovementVector.Size() == 0 && (AnimationState == EAnimationState::Walking || AnimationState == EAnimationState::Running))
		{
			SetState(EAnimationState::Idle);
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
	else if (AnimationState == EAnimationState::Running)
	{
		DominantDriver = TrotPoseDriver;
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
