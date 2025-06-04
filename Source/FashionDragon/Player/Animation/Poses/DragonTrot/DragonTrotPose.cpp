#include "DragonTrotPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Components/WalkCyclePoseComponent.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverGroundRootSway.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverGroundHipSway.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverTurnToMovement.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Drivers/DragonIdleWingDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonSprint/DragonSprintPose.h"

#define STEP_DURATION 0.9f
#define PLANTED_DURATION STEP_DURATION * 0.6f
#define INERTIA_DURATION STEP_DURATION - PLANTED_DURATION

// ============================================================================
// Pose
// ============================================================================

FDragonTrotPose::FDragonTrotPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	RootDriver = new FDragonDriverGroundRootSway(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));

	const auto RootTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::RotateToMovement));
	RootTurnDriver->SetAxisMask(0.5f, 0.0f, 0.0f);
	
	const auto NeckTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledHead.GetBone(EDriverLayer::RotateToMovement));
	NeckTurnDriver->SetAxisMask(FVector(-1, 1, 0));

	const auto BodyTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledBody.GetBone(EDriverLayer::RotateToMovement));
	BodyTurnDriver->SetAxisMask(FVector(0, 2, 0));

	HipsDriver = new FDragonDriverGroundHipSway(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Primary),  Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	BoneDrivers = { RootDriver, RootTurnDriver, NeckTurnDriver, BodyTurnDriver, HipsDriver };
	
	LeftLegDriver = new FDragonTrotLegDriver(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary));
	RightLegDriver = new FDragonTrotLegDriver(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};

	RootDriver->SetHorizontalAmplitude(5.0f);
	RootDriver->SetVerticalOffset(120.0f);
	RootDriver->SetVerticalAmplitude(220.0f);

	LeftLegDriver->OnWalkStateChanged.AddLambda([this](ELegWalkingState, const ELegWalkingState NewState)
	{
		if (NewState == ELegWalkingState::Stepping)
			RootDriver->SetLeadingLeg(LeftLegDriver);
	});
	RightLegDriver->OnWalkStateChanged.AddLambda([this](ELegWalkingState, const ELegWalkingState NewState)
	{
		if (NewState == ELegWalkingState::Stepping)
			RootDriver->SetLeadingLeg(RightLegDriver);
	});

	// Shares a driver with idle animation
	WingDrivers = {
		new FDragonIdleWingDriver(Anim, Anim->LeftWing.GetBone(EDriverLayer::Primary)),
		new FDragonIdleWingDriver(Anim, Anim->RightWing.GetBone(EDriverLayer::Primary)),
	};

	WalkCycleComponent = new FWalkCyclePoseComponent(this, LeftLegDriver, RightLegDriver);
	WalkCycleComponent->SetCycleBreakpoints({
		FBreakpoint(STEP_DURATION, ELegWalkingState::Stepping),
		FBreakpoint(PLANTED_DURATION, ELegWalkingState::Planted),
		FBreakpoint(INERTIA_DURATION, ELegWalkingState::Inertia),
	});
	Components = {
		WalkCycleComponent,
	};
}

void FDragonTrotPose::SyncStateFrom(const FDragonWalkPose* SourcePose) const
{
	// BodyDriver->SyncStateFrom(SourcePose->BodyDriver);
	// HipsDriver->SyncStateFrom(SourcePose->HipsDriver);
	LeftLegDriver->SyncStateFrom(SourcePose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(SourcePose->RightLegDriver);

	const auto LeadingLeg = LeftLegDriver->WalkingState == ELegWalkingState::Stepping ? LeftLegDriver : RightLegDriver;
	const auto OtherLeg = LeadingLeg == LeftLegDriver ? RightLegDriver : LeftLegDriver;
	if (LeadingLeg->GetCyclePosition() < PLANTED_DURATION)
	{
		OtherLeg->LockToWorldGround();
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition());
	}
	else
	{
		OtherLeg->SetWalkingState(ELegWalkingState::Inertia);
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition() - PLANTED_DURATION);
	}

	WalkCycleComponent->SyncStateFrom(SourcePose->WalkCycleComponent);
}

void FDragonTrotPose::SyncStateFrom(const FDragonSprintPose* SourcePose) const
{
	// BodyDriver->SyncStateFrom(SourcePose->BodyDriver);
	// HipsDriver->SyncStateFrom(SourcePose->HipsDriver);
	LeftLegDriver->SyncStateFrom(SourcePose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(SourcePose->RightLegDriver);

	const auto LeadingLeg = LeftLegDriver->WalkingState == ELegWalkingState::Stepping ? LeftLegDriver : RightLegDriver;
	const auto OtherLeg = LeadingLeg == LeftLegDriver ? RightLegDriver : LeftLegDriver;
	if (LeadingLeg->GetCyclePosition() < PLANTED_DURATION)
	{
		OtherLeg->LockToWorldGround();
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition());
	}
	else
	{
		OtherLeg->SetWalkingState(ELegWalkingState::Inertia);
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition() - PLANTED_DURATION);
	}

	WalkCycleComponent->SyncStateFrom(SourcePose->WalkCycleComponent);
}

void FDragonTrotPose::ResetState()
{
	RootDriver->ResetState();
	LeftLegDriver->LockToWorldGround();
	RightLegDriver->SetWalkingState(ELegWalkingState::Stepping);

	WalkCycleComponent->ResetState();
}

// ============================================================================
// Leg Driver
// ============================================================================

FDragonTrotLegDriver::FDragonTrotLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegSteppingDriver(AnimInstance, ControlledLeg)
{
	BlendSpeed = 0.5f;
	BlendMode = EDriverBlend::EaseOut;
}

void FDragonTrotLegDriver::Tick(const float DeltaTime)
{
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;

	FProceduralLegSteppingDriver::Tick(AdvanceValue);

	// If the leg is stretched too far, disconnect
	if (WalkingState == ELegWalkingState::Planted && Leg->Position.Size() > 350.0f && Leg->Position.Y < 0.0f)
	{
		SetWalkingState(ELegWalkingState::Raised);
	}
}

void FDragonTrotLegDriver::AdvanceState()
{
	switch (WalkingState)
	{
	case ELegWalkingState::Relaxed:
		SetWalkingState(ELegWalkingState::Raised);
		break;
	case ELegWalkingState::Raised:
		SetWalkingState(ELegWalkingState::Inertia);
		break;
	case ELegWalkingState::Inertia:
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Planted:
	case ELegWalkingState::SeekingGround:
		SetWalkingState(ELegWalkingState::Inertia);
		break;
	case ELegWalkingState::Stepping:
		LockToWorldGround();
		break;
	}
}

FDragonWalkStateData FDragonTrotLegDriver::GetRawWalkStateData() const
{
	const std::map<ELegWalkingState, FDragonWalkStateData> AnimData =
	{
		{ ELegWalkingState::Relaxed,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 2.5,
				.AngularForce = 1.0f,
				.Duration = 1.0f,
			}
		},
		{ ELegWalkingState::Raised,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 150.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.LinearForce = 0.7f,
				.AngularForce = 0.1f,
				.Duration = 1.0f,
			}
		},
		{ ELegWalkingState::SeekingGround,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 5.0f,
				.AngularForce = 0.03f,
				.Duration = 1.0f,
			}
		},
		{ ELegWalkingState::Planted,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 10.f,
				.AngularForce = 1.0f,
				.Duration = PLANTED_DURATION,
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(0.0f, 280.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 8000.0f,
				.AngularForce = 1.0f,
				.Duration = STEP_DURATION,
				.StartArticulationPosition = FVector(0.0f, 0.0f, 150.0f),
				.StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f),
				.EndArticulationPosition = FVector(0.0f, 0.0f, 10.0f),
				.EndArticulationRotation = FVector(0.0f, 0.0f, 10.0f),
			}
		},
		{ ELegWalkingState::Inertia,
			{
				.TargetPosition = FVector(0.0f, -450.0f, 100.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 70.0f),
				.LinearForce = 3.0f,
				.AngularForce = 0.2f,
				.Duration = INERTIA_DURATION,
			}
		},
	};
	
	return AnimData.at(WalkingState);
}
