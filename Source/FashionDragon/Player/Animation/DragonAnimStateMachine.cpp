#include "DragonAnimStateMachine.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void FDragonAnimStateMachine::InitTransitions()
{
	Transitions = {
		{ Idle, {
			{ Walking, [this]()
			{
				WalkPoseDriver->ResetState();
				WalkPoseDriver->BlendAlpha = 1.0f;
			}},
			{ Running, [this]()
			{
				TrotPoseDriver->ResetState();
				TrotPoseDriver->BlendAlpha = 1.0f;
			}},
			{ Jumping, [this]()
			{
				JumpPoseDriver->ResetState();
				// JumpPoseDriver->BlendAlpha = 1.0f;
			}},
		}},
		{ Jumping, {
			{ Idle, [this]()
			{
				WalkPoseDriver->ResetState();
				JumpPoseDriver->BlendAlpha = 0.0f;
			}},
			{ Walking, [this]()
			{
				WalkPoseDriver->ResetState();
				JumpPoseDriver->BlendAlpha = 0.0f;
				WalkPoseDriver->BlendAlpha = 1.0f;
			}},
			{ Running, [this]()
			{
				TrotPoseDriver->ResetState();
				JumpPoseDriver->BlendAlpha = 0.0f;
				TrotPoseDriver->BlendAlpha = 1.0f;
			}},
		}},
		{ Walking, {
			{ Idle, [this]() { IdlePoseDriver->SyncStateFrom(WalkPoseDriver); } },
			{ Running, [this]() { TrotPoseDriver->SyncStateFrom(WalkPoseDriver); } },
			{ Jumping, [this]()
			{
				JumpPoseDriver->ResetState();
				JumpPoseDriver->BlendAlpha = 1.0f;
			} },
		}},
		{ Running, {
			{ Walking, [this]() { WalkPoseDriver->SyncStateFrom(TrotPoseDriver); } },
			{ Jumping, [this]()
			{
				JumpPoseDriver->ResetState();
				JumpPoseDriver->BlendAlpha = 1.0f;
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
	if (AnimationState != Jumping && (OwningActor->IsChargingJump))
	{
		SetState(Jumping);
		AnimationLockout = 0.5f;
	}
	else if (AnimationState == Jumping && (OwningActor->IsChargingJump))
	{
		AnimationLockout = 0.5f;
	}

	if (AnimationLockout <= 0.0f)
	{
		if (AnimationState == Jumping && !OwningActor->IsChargingJump && !OwningActor->GetCharacterMovement()->IsFalling())
		{
			SetState(Idle);
		}
		if (MovementVector.Size() > 0 && !OwningActor->IsSprinting && (AnimationState == Idle || AnimationState == Running))
		{
			SetState(Walking);
		}
		if (MovementVector.Size() > 0 && OwningActor->IsSprinting && (AnimationState == Idle || AnimationState == Walking))
		{
			SetState(Running);
		}
		if (MovementVector.Size() == 0 && (AnimationState == Walking || AnimationState == Running))
		{
			SetState(Idle);
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
	if (AnimationState == Jumping)
	{
		DominantDriver = JumpPoseDriver;
	}
	else if (AnimationState == Idle)
	{
		DominantDriver = IdlePoseDriver;
	}
	else if (AnimationState == Walking)
	{
		DominantDriver = WalkPoseDriver;
	}
	else if (AnimationState == Running)
	{
		DominantDriver = TrotPoseDriver;
	}

	DominantDriver->BlendAlpha += DeltaTime * 100.0f;
	if (DominantDriver->BlendAlpha > 1.0f)
		DominantDriver->BlendAlpha = 1.0f;
	
	for (FProceduralPose* PoseDriver : PoseDrivers)
	{
		if (PoseDriver != DominantDriver)
		{
			PoseDriver->BlendAlpha -= DeltaTime * 1000.0f;
			if (PoseDriver->BlendAlpha < 0.0f)
				PoseDriver->BlendAlpha = 0.0f;
		}
	}

	// Foot placement always applies
	FootPlacementDriver->BlendAlpha = 1.0f;
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
