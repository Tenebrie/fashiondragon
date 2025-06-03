#include "DragonSprintPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Components/WalkCyclePoseComponent.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverGroundRootSway.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverGroundHipSway.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverTurnToMovement.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Drivers/DragonIdleWingDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonTrot/DragonTrotPose.h"

FDragonSprintLegDriver::FDragonSprintLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegSteppingDriver(AnimInstance, ControlledLeg)
{
	BlendMode = EDriverBlend::EaseOut;
}

// ============================================================================
// Leg Driver
// ============================================================================

void FDragonSprintLegDriver::Tick(const float DeltaTime)
{
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;

	FProceduralLegSteppingDriver::Tick(AdvanceValue);

	// If the leg is stretched too far, disconnect
	if (WalkingState == ELegWalkingState::Planted && Leg->Position.Size() > 350.0f && Leg->Position.Y < 0.0f)
	{
		// SetWalkingState(ELegWalkingState::Inertia, true);
	}
}

void FDragonSprintLegDriver::AdvanceState()
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
		SetWalkingState(ELegWalkingState::Inertia);
		break;
	case ELegWalkingState::Stepping:
		LockToWorldGround();
		break;
	}
}

// inline constexpr float StepDuration = 1.2f;
// inline constexpr float PlantedDuration = 0.7f;
// inline constexpr float InertiaDuration = 0.5f;
#define STEP_DURATION 1.2f
#define PLANTED_DURATION STEP_DURATION * 0.4f
#define INERTIA_DURATION STEP_DURATION - PLANTED_DURATION
FDragonWalkStateData FDragonSprintLegDriver::GetRawWalkStateData() const
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
				.Duration = PLANTED_DURATION
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(0.0f, 400.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 80.0f,
				.AngularForce = 1.0f,
				.Duration = STEP_DURATION,
				.StartArticulationPosition = FVector(0.0f, 0.0f, 100.0f),
				.StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f),
				.EndArticulationPosition = FVector(0.0f, 0.0f, 30.0f),
				.EndArticulationRotation = FVector(0.0f, 0.0f, 10.0f),
			}
		},
		{ ELegWalkingState::Inertia,
			{
				.TargetPosition = FVector(0.0f, -500.0f, 180.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 90.0f),
				.LinearForce = 15.0f,
				.AngularForce = 5.0f,
				.Duration = INERTIA_DURATION
			}
		},
	};
	
	return AnimData.at(WalkingState);
}

FDragonSprintPose::FDragonSprintPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	RootDriver = new FDragonDriverGroundRootSway(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	RootDriver->SetVerticalOffset(200);
	RootDriver->SetVerticalAmplitude(350);
	
	const auto RootTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::RotateToMovement));
	RootTurnDriver->SetAxisMask(0.75f, 0.0f, 0.0f);

	const auto BodyTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledBody.GetBone(EDriverLayer::RotateToMovement));
	BodyTurnDriver->SetAxisMask(FVector(0, 1.25, 0));
	
	HipsDriver = new FDragonDriverGroundHipSway(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Primary),  Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	
	BoneDrivers = { RootDriver, RootTurnDriver, BodyTurnDriver, HipsDriver };
	
	LeftLegDriver = new FDragonSprintLegDriver(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary));
	RightLegDriver = new FDragonSprintLegDriver(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};

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

void FDragonSprintPose::SyncStateFrom(const FDragonWalkPose* SourcePose) const
{
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

void FDragonSprintPose::SyncStateFrom(const FDragonTrotPose* SourcePose) const
{
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

void FDragonSprintPose::ResetState()
{
	RootDriver->ResetState();
	HipsDriver->ResetState();
	LeftLegDriver->LockToWorldGround();
	RightLegDriver->SetWalkingState(ELegWalkingState::Stepping);

	WalkCycleComponent->ResetState();
}
