#include "DragonWalkPose.h"

#include <map>

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Drivers/DragonIdleWingDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonTrot/DragonTrotPose.h"
#include "FashionDragon/Utils/Utils.h"

// ============================================================================
// Body Driver
// ============================================================================

void FDragonWalkBodyDriver::Tick(const float DeltaTime)
{
	BlendAlpha = FMath::FInterpTo(BlendAlpha, TargetBlendAlpha, DeltaTime, 1.0f);
	
	const auto LeftLegOffset = std::min(1.0, LeftLeg->Position.Size() / 750.0f);
	const auto RightLegOffset = std::min(1.0, RightLeg->Position.Size() / 750.0f);

	const auto LegState = std::min(LeftLegOffset, RightLegOffset);
	const auto VerticalOffset = LegState * -50.0f + 40.0f;

	DesiredPosition = FVector(0.0f, 0.0f, VerticalOffset);

	// Lerp current value to target value
	auto TargetLean = -10.0f;

	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (OwningActor->IsSprinting)
	{
		TargetLean += 10.0f;
	}

	const auto InputRotation = GetInputRotation();
	auto Bank = FMath::Sin(InputRotation) * 10.0f;
	if (AnimInstance->StateMachine->AnimationState == EAnimationState::Jumping)
	{
		Bank = 0.0f;
	}

	DesiredForce = 25.0f;
	DesiredRotation = FRotator(-Bank, -Bank * 0.25, TargetLean);
	// DesiredRotation = FRotator(0.0f, 0.0f, 0.0f);
}

void FDragonWalkBodyDriver::SyncStateFrom(const FDragonWalkBodyDriver* TargetDriver)
{
	DesiredPosition = TargetDriver->DesiredPosition;
	DesiredRotation = TargetDriver->DesiredRotation;
	CyclePosition = TargetDriver->CyclePosition;
	VisualCyclePosition = TargetDriver->VisualCyclePosition;
	ArticulationPosition = TargetDriver->ArticulationPosition;
	ArticulationRotation = TargetDriver->ArticulationRotation;
}

// ============================================================================
// Hip Sway Driver
// ============================================================================

void FDragonWalkHipSwayDriver::Tick(const float DeltaTime)
{
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();
	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;
	
	FProceduralBoneDriver::Tick(AdvanceValue);

	// TODO: Calculate the sway value based on the position of each leg.
	// Specifically, a leg that's in Planted state contributes negative weight, while a leg in Stepping state is positive.
	const auto LeftLegOffset = std::min(1.0, LeftLeg->Position.Y / 750.0f);
	const auto RightLegOffset = -std::min(1.0, RightLeg->Position.Y / 750.0f);
	const auto Value = LeftLegOffset + RightLegOffset;

	const auto Sway = FMath::Sin(Value * PI) * 8.0f;
	
	DesiredRotation = FRotator(0.0f, Sway, Sway);
}

void FDragonWalkHipSwayDriver::SyncStateFrom(const FDragonWalkHipSwayDriver* TargetDriver)
{
	DesiredPosition = TargetDriver->DesiredPosition;
	DesiredRotation = TargetDriver->DesiredRotation;
	CyclePosition = TargetDriver->CyclePosition;
	VisualCyclePosition = TargetDriver->VisualCyclePosition;
}

// ============================================================================
// Leg Driver
// ============================================================================

FDragonWalkLegDriver::FDragonWalkLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegDriver(AnimInstance, ControlledLeg)
{}

void FDragonWalkLegDriver::AdvanceState()
{
	switch (WalkingState)
	{
	case ELegWalkingState::Relaxed:
		SetWalkingState(ELegWalkingState::Raised);
		break;
	case ELegWalkingState::Raised:
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Inertia:
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Planted:
	case ELegWalkingState::SeekingGround:
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Stepping:
		LockToWorldGround();
		break;
	default:
		break;
	}
}

FDragonWalkStateData FDragonWalkLegDriver::GetRawWalkStateData() const
{
	const std::map<ELegWalkingState, FDragonWalkStateData> AnimData =
	{
		{ ELegWalkingState::Relaxed,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 2.5,
				.AngularForce = 1.0f,
				.Duration = 1.0f
			}
		},
		{ ELegWalkingState::Raised,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 150.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.LinearForce = 0.7f,
				.AngularForce = 0.1f,
				.Duration = 0.3f
			}
		},
		{ ELegWalkingState::SeekingGround,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 5.0f,
				.AngularForce = 0.03f,
				.Duration = 0.3f
			}
		},
		{ ELegWalkingState::Planted,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 10.f,
				.AngularForce = 1.0f,
				.Duration = 0.9f
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(-20.0f, 300.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 15.0f, 0.0f),
				.LinearForce = 2.0f,
				.AngularForce = 1.0f,
				.Duration = 0.9f,
				.StartArticulationPosition = FVector(15.0f, 0.0f, 150.0f),
				.StartArticulationRotation = FVector(0.0f, 0.0f, 60.0f),
				.EndArticulationPosition = FVector(15.0f, 0.0f, 30.0f),
				.EndArticulationRotation = FVector(0.0f, 0.0f, 10.0f),
			}
		},
		{ ELegWalkingState::Inertia,
			{
				.TargetPosition = FVector(0.0f, -250.0f, 200.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.LinearForce = 2.0f,
				.AngularForce = 1.0f,
				.Duration = 0.5f
			}
		},
	};
	
	return AnimData.at(WalkingState);
}

void FDragonWalkLegDriver::Tick(const float DeltaTime)
{
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;

	FProceduralLegDriver::Tick(AdvanceValue);
	
	// If the leg is stretched too far, disconnect
	const auto LegReference = RotateVectorToInputRotation(Leg->Position, true);
	const auto ShouldDisconnect = FMath::Abs(LegReference.X) > 200.f || LegReference.Z < -150.0f || LegReference.Y > 600.0f || LegReference.Y < -300.0f
		|| FUtils::GetRotatorDistance(Leg->Rotation) > 50.0f;
	if (WalkingState == ELegWalkingState::Planted && ShouldDisconnect)
	{
		SetWalkingState(ELegWalkingState::Raised, true);
	}
}

FDragonWalkPose::FDragonWalkPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDriver = new FDragonWalkBodyDriver(Anim, Anim->ControlledBody.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	HipsDriver = new FDragonWalkHipSwayDriver(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	BodyDrivers = { BodyDriver };
	HipsDrivers = { HipsDriver };

	LeftLegDriver = new FDragonWalkLegDriver(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary));
	RightLegDriver = new FDragonWalkLegDriver(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};

	// Shares a driver with idle animation
	WingDrivers = {
		new FDragonIdleWingDriver(Anim, Anim->LeftWing.GetBone(EDriverLayer::Primary)),
		new FDragonIdleWingDriver(Anim, Anim->RightWing.GetBone(EDriverLayer::Primary)),
	};
}

void FDragonWalkPose::SyncStateFrom(const FDragonTrotPose* TargetPose) const
{
	// BodyDriver->SyncStateFrom(TargetPose->BodyDriver);
	// HipsDriver->SyncStateFrom(TargetPose->HipsDriver);
	LeftLegDriver->SyncStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(TargetPose->RightLegDriver);
}

// TODO: If it's not been enough time since we have been here, don't reset, but continue the previous state
void FDragonWalkPose::ResetState()
{
	BodyDriver->ResetState();
	HipsDriver->ResetState();

	const auto PushingLeg = SwitchStartingLeg ? RightLegDriver : LeftLegDriver;
	const auto SteppingLeg = SwitchStartingLeg ? LeftLegDriver : RightLegDriver;

	PushingLeg->LockToWorldGround();
	SteppingLeg->SetWalkingState(ELegWalkingState::Stepping);

	SwitchStartingLeg = !SwitchStartingLeg;
}
