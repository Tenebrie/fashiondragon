#include "DragonAnimInstance.h"

#include <map>

#include "ControlledLeg.h"
#include "../MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Poses/DragonIdlePose.h"
#include "Poses/DragonTrotPose.h"
#include "Poses/DragonWalkPose.h"
#include "Poses/DragonJumpPose.h"

#define PRINT(Text, ...) \
if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT(Text), ##__VA_ARGS__))

/**
 * @brief Init
 */
void UDragonAnimInstance::NativeInitializeAnimation()
{
	LegPositions = TArray<FVector>();
	LegRotations = TArray<FRotator>();
	for (int i = 0; i < 4; i++)
	{
		LegPositions.Add(FVector(0.0f, 0.0f, 0.0f));
		LegRotations.Add(FRotator(0.0f, 0.0f, 0.0f));
	}
	
	ControlledLegs = TArray<FControlledLeg*>();
	// BackLeftPaw
	ControlledLegs.Add(new FControlledLeg(this, 0));
	// BackRightPaw
	ControlledLegs.Add(new FControlledLeg(this, 1));

	IdlePoseDriver = new FDragonIdlePose(this, ControlledLegs[0], ControlledLegs[1]);
	WalkPoseDriver = new FDragonWalkPose(this, ControlledLegs[0], ControlledLegs[1]);
	TrotPoseDriver = new FDragonTrotPose(this, ControlledLegs[0], ControlledLegs[1]);
	JumpPoseDriver = new FDragonJumpPose(this, ControlledLegs[0], ControlledLegs[1]);
	
	PoseDrivers = TArray<FAbstractProceduralPose*>();
	PoseDrivers.Add(IdlePoseDriver);
	PoseDrivers.Add(WalkPoseDriver);
	PoseDrivers.Add(TrotPoseDriver);
	PoseDrivers.Add(JumpPoseDriver);
}

/**
 * @brief Every frame, update the animation state and leg positions.
 */
void UDragonAnimInstance::NativeUpdateAnimation(const float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	AnimationLockout -= DeltaTime;
	if (AnimationLockout < 0.0f)
		AnimationLockout = 0.0f;
	
	const auto OwningActor = Cast<AMainCharacter>(GetOwningActor());
	if (!OwningActor)
		return;

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
		if (MovementVector.Size() > 0 && AnimationState == EAnimationState::Idle)
		{
			SetState(Walking);
		}
		if (MovementVector.Size() == 0 && AnimationState == EAnimationState::Walking)
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
	UpdateWalkingBobCycle();

	for (int i = 0; i < ControlledLegs.Num(); i++)
	{
		TArray<FPoseEffector> Effectors;

		// const FAbstractProceduralPose* DominantDriver = IdlePoseDriver;

		/**
		 * Serious TODO:
		 * Effectors should not live in their personal worlds. Instead, they should be desires,
		 * actions, forces. Basically, instead of them simulating their state in the isolated bubble,
		 * they should look at the leg's ACTUAL current position and push it towards the destination.
		 * They can simulate their "desired" target based on the bezier and the time, but the blending
		 * shouldn't just take average of positions. Instead, it should just instantly switch to another
		 * active effector which can now hard push in the direction it wants to go.
		 * Add some inertia effector as well.
		 * And hopefully the ground IK will be represented as an effector?
		 */
		for (const auto PoseDriver : PoseDrivers)
		{
			const auto Effector = PoseDriver->ToLegEffector(ControlledLegs[i]);
			if (Effector.Weight > 0.0f)
			{
				Effectors.Add(Effector);
			}
		}

		// Merge the positions and rotations of all effectors
		FVector Position = FVector(0.0f, 0.0f, 0.0f);
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
		for (const auto& [Weight, Pos, Rot] : Effectors)
		{
			Position += Pos * Weight;
			Rotation += Rot * Weight;
		}

		const auto Leg = ControlledLegs[i];
		Leg->Position = Position;
		Leg->Rotation = Rotation;
		LegPositions[i] = Leg->Position - FVector(0.0f, 0.0f, 0.0f);
		LegRotations[i] = Leg->Rotation;
	}
}

/**
 * @brief Merge the drivers' current positions and apply them to controlled entities.
 */
void UDragonAnimInstance::BlendDrivers(const float DeltaTime) const
{
	FAbstractProceduralPose* DominantDriver = IdlePoseDriver;
	const auto OwningActor = Cast<AMainCharacter>(GetOwningActor());
	if (AnimationState == Jumping)
	{
		DominantDriver = JumpPoseDriver;
	}
	else if (AnimationState == Idle)
	{
		DominantDriver = IdlePoseDriver;
	}
	else if (AnimationState == Walking && !OwningActor->IsSprinting)
	{
		DominantDriver = WalkPoseDriver;
	}
	else if (AnimationState == Walking && OwningActor->IsSprinting)
	{
		DominantDriver = TrotPoseDriver;
	}

	DominantDriver->BlendAlpha += DeltaTime * 10.0f;
	if (DominantDriver->BlendAlpha > 1.0f)
		DominantDriver->BlendAlpha = 1.0f;
	
	for (FAbstractProceduralPose* PoseDriver : PoseDrivers)
	{
		if (PoseDriver != DominantDriver)
		{
			PoseDriver->BlendAlpha -= DeltaTime * 10.0f;
			if (PoseDriver->BlendAlpha < 0.0f)
				PoseDriver->BlendAlpha = 0.0f;
		}
	}
}

/**
 * @brief Update the walking bob cycle based on the leg's position.
 */
void UDragonAnimInstance::UpdateWalkingBobCycle()
{
	// TODO: Rewrite bobbing
	// if (AnimationState != Walking)
	// {
	// 	WalkingBobCycle = 0.0f;
	// 	return;
	// }
	//
	// FControlledLeg* Leg;
	// if (ControlledLegs[1]->GetState() == ELegWalkingState::Planted)
	// 	Leg = ControlledLegs[1];
	// else if (ControlledLegs[0]->GetState() == ELegWalkingState::Planted)
	// 	Leg = ControlledLegs[0];
	// else
	// {
	// 	return;
	// }
	//
	// const auto OffsetFromOrigin = std::min(1.0, Leg->Position.Size() / 300.0f);
	// // Print offset
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
	// 	FString::Printf(TEXT("Offset: %s"), *FString::SanitizeFloat(OffsetFromOrigin)));
	//
	// WalkingBobCycle = (1.0f - OffsetFromOrigin * 2.0f) * 15.0f;
}

/**
 * @brief Switches the animation state in response to player's input.
 */
void UDragonAnimInstance::SetState(const EAnimationState State)
{
	this->AnimationState = State;

	if (State == Walking)
	{
		// ControlledLegs[0]->RequestState(Stepping);
		// ControlledLegs[1]->RequestState(Planted);
		WalkPoseDriver->ResetState();
	}
	if (State == Idle)
	{
		// ControlledLegs[0]->RequestState(Relaxed);
		// ControlledLegs[1]->RequestState(Relaxed);
		IdlePoseDriver->ResetState();
	}
	if (State == Jumping)
	{
		JumpPoseDriver->ResetState();
	}
}
