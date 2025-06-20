#include "DragonAnimStateMachine.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/InputHandlers/FlightHandler.h"
#include "FashionDragon/Utils/Utils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Poses/DragonFootPlacement/DragonFootPlacementPose.h"
#include "Poses/DragonFlight/DragonFlightPose.h"
#include "Poses/DragonIdle/DragonIdlePose.h"
#include "Poses/DragonJump/DragonJumpPose.h"
#include "Poses/DragonRandomSway/DragonRandomSwayPose.h"
#include "Poses/DragonNull/DragonNullPose.h"
#include "Poses/DragonTrot/DragonTrotPose.h"
#include "Poses/DragonWalk/DragonWalkPose.h"
#include "Poses/DragonSprint/DragonSprintPose.h"
#include "Poses/DragonMomentum/DragonMomentumPose.h"

FDragonAnimStateMachine::FDragonAnimStateMachine(
	FDragonNullPose* NullPoseDriver,
	FDragonIdlePose* IdlePoseDriver,
	FDragonWalkPose* WalkPoseDriver,
	FDragonTrotPose* TrotPoseDriver,
	FDragonSprintPose* SprintPoseDriver,
	FDragonJumpPose* JumpPoseDriver,
	FDragonFlightPose* FlightPoseDriver,
	FDragonRandomSwayPose* RandomSwayDriver,
	FDragonMomentumPose* MomentumDriver,
	FDragonFootPlacementPose* FootPlacementDriver
):
	NullPoseDriver(NullPoseDriver),
	IdlePoseDriver(IdlePoseDriver),
	WalkPoseDriver(WalkPoseDriver),
	TrotPoseDriver(TrotPoseDriver),
	SprintPoseDriver(SprintPoseDriver),
	JumpPoseDriver(JumpPoseDriver),
	FlightPoseDriver(FlightPoseDriver),
	RandomSwayDriver(RandomSwayDriver),
	MomentumDriver(MomentumDriver),
	FootPlacementDriver(FootPlacementDriver)
{
	PoseDrivers = {
		NullPoseDriver,
		IdlePoseDriver,
		WalkPoseDriver,
		TrotPoseDriver,
		SprintPoseDriver,
		JumpPoseDriver,
		FlightPoseDriver,
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
				WalkPoseDriver->LeftLegDriver->ForceSetBlendAlpha(1.0f);
				WalkPoseDriver->RightLegDriver->ForceSetBlendAlpha(1.0f);
			}},
			{ EAnimationState::Trotting, [this]
			{
				TrotPoseDriver->ResetState();
				TrotPoseDriver->LeftLegDriver->ForceSetBlendAlpha(1.0f);
				TrotPoseDriver->RightLegDriver->ForceSetBlendAlpha(1.0f);
			}},
			{ EAnimationState::Sprinting, [this]
			{
				SprintPoseDriver->ResetState();
				SprintPoseDriver->LeftLegDriver->ForceSetBlendAlpha(1.0f);
				SprintPoseDriver->RightLegDriver->ForceSetBlendAlpha(1.0f);
			}},
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->SyncStateFrom(IdlePoseDriver);
			}},
		}},
		{ EAnimationState::Jumping, {
			{ EAnimationState::Idle, [this] { IdlePoseDriver->ResetState(); }},
			{ EAnimationState::Walking, [this] { WalkPoseDriver->ResetState(); }},
			{ EAnimationState::Trotting, [this] { TrotPoseDriver->ResetState(); }},
			{ EAnimationState::Sprinting, [this] { SprintPoseDriver->ResetState(); }},
			{ EAnimationState::Jumping, [this] { JumpPoseDriver->ResetState(); }},
		}},
		{ EAnimationState::Walking, {
			{ EAnimationState::Idle, [this]
			{
				IdlePoseDriver->SyncStateFrom(WalkPoseDriver);
				WalkPoseDriver->LeftLegDriver->ForceSetBlendAlpha(0.0f);
				WalkPoseDriver->RightLegDriver->ForceSetBlendAlpha(0.0f);
			}},
			{ EAnimationState::Trotting, [this] { TrotPoseDriver->SyncStateFrom(WalkPoseDriver); }},
			{ EAnimationState::Sprinting, [this] { SprintPoseDriver->SyncStateFrom(WalkPoseDriver); }},
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->SyncStateFrom(WalkPoseDriver);
				WalkPoseDriver->LeftLegDriver->ForceSetBlendAlpha(0.0f);
				WalkPoseDriver->RightLegDriver->ForceSetBlendAlpha(0.0f);
			}},
		}},
		{ EAnimationState::Trotting, {
			{ EAnimationState::Idle, [this]
			{
				IdlePoseDriver->SyncStateFrom(TrotPoseDriver);
				TrotPoseDriver->LeftLegDriver->ForceSetBlendAlpha(0.0f);
				TrotPoseDriver->RightLegDriver->ForceSetBlendAlpha(0.0f);
			}},
			{ EAnimationState::Walking, [this] { WalkPoseDriver->SyncStateFrom(TrotPoseDriver); } },
			{ EAnimationState::Sprinting, [this] { SprintPoseDriver->SyncStateFrom(TrotPoseDriver); } },
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->SyncStateFrom(TrotPoseDriver);
				TrotPoseDriver->LeftLegDriver->ForceSetBlendAlpha(0.0f);
				TrotPoseDriver->RightLegDriver->ForceSetBlendAlpha(0.0f);
			}},
		}},
		{ EAnimationState::Sprinting, {
			{ EAnimationState::Idle, [this]
			{
				IdlePoseDriver->SyncStateFrom(SprintPoseDriver);
				SprintPoseDriver->LeftLegDriver->ForceSetBlendAlpha(0.0f);
				SprintPoseDriver->RightLegDriver->ForceSetBlendAlpha(0.0f);
			}},
			{ EAnimationState::Walking, [this] { WalkPoseDriver->SyncStateFrom(SprintPoseDriver); } },
			{ EAnimationState::Trotting, [this] { TrotPoseDriver->SyncStateFrom(SprintPoseDriver); } },
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->SyncStateFrom(SprintPoseDriver);
				SprintPoseDriver->LeftLegDriver->ForceSetBlendAlpha(0.0f);
				SprintPoseDriver->RightLegDriver->ForceSetBlendAlpha(0.0f);
			}},
		}},
		{ EAnimationState::Flight, {
			{ EAnimationState::Jumping, [this]
			{
				JumpPoseDriver->StartFalling();
			}},
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
		if (MovementVector.Size() > 0 && CanSwitchWalkAnim && AnimationState != EAnimationState::Walking && OwningActor->MovementMode == EGroundMovementMode::Walking)
		{
			SetState(EAnimationState::Walking);
		}
		if (MovementVector.Size() > 0 && CanSwitchWalkAnim && AnimationState != EAnimationState::Trotting && OwningActor->MovementMode == EGroundMovementMode::Trotting)
		{
			SetState(EAnimationState::Trotting);
		}
		if (MovementVector.Size() > 0 && CanSwitchWalkAnim && AnimationState != EAnimationState::Sprinting && OwningActor->MovementMode == EGroundMovementMode::Sprinting)
		{
			SetState(EAnimationState::Sprinting);
		}
		if (AnimationState != EAnimationState::Jumping && OwningActor->GetCharacterMovement()->IsFalling())
		{
			SetState(EAnimationState::Jumping);
			JumpPoseDriver->StartFalling();
		}
		if (AnimationState != EAnimationState::Flight && OwningActor->FlightHandler->IsFlying())
		{
			SetState(EAnimationState::Flight);
			FlightPoseDriver->ResetState();
		}
		if (AnimationState == EAnimationState::Flight && !OwningActor->FlightHandler->IsFlying())
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
	else if (AnimationState == EAnimationState::Flight)
	{
		DominantDriver = FlightPoseDriver;
	}

	DominantDriver->AddBlendAlpha(DeltaTime * 1000.0f);
	
	for (FProceduralPose* PoseDriver : PoseDrivers)
	{
		if (PoseDriver != DominantDriver)
		{
			PoseDriver->AddBlendAlpha(-DeltaTime * 1000.0f);
		}
	}
	
	MomentumDriver->SetBlendAlpha(1.0f);
	RandomSwayDriver->SetBlendAlpha(1.0f);
	FootPlacementDriver->SetBlendAlpha(1.0f);

	NullPoseDriver->EvaluateBlending();
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
