#include "DragonTrotPose.h"

#include <map>

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Drivers/DragonIdleWingDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonSprint/DragonSprintPose.h"

FDragonTrotLegDriver::FDragonTrotLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegDriver(AnimInstance, ControlledLeg)
{}

// ============================================================================
// Body Driver
// ============================================================================

void FDragonTrotBodyDriver::Tick(const float DeltaTime)
{
	BlendAlpha = FMath::FInterpTo(BlendAlpha, TargetBlendAlpha, DeltaTime, 5.0f);

	if (LeadingLeg)
	{
		const auto LeadingLegOffset = std::min(1.0, LeadingLeg->GetLeg()->Position.Size() / 750.0f);
		// const auto LeftLegOffset = std::min(1.0, LeftLeg->Position.Size() / 750.0f);
		// const auto RightLegOffset = std::min(1.0, RightLeg->Position.Size() / 750.0f);

		const auto SideOffsetSign = LeadingLeg->GetLeg()->GetIdx() == 0 ? 1.0f : -1.0f;
	
		const auto ForwardOffset = FMath::Max(400.0f - LeadingLeg->GetLeg()->Position.Y, 0.0f) / 15.0f;
		const auto VerticalOffset = LeadingLegOffset * -175.0f;

		const auto LastPos = DesiredPosition - FVector(0, 0, 100);
		DesiredPosition = FMath::VInterpTo(LastPos, FVector(SideOffsetSign * 10, ForwardOffset, VerticalOffset), DeltaTime, 5.5f);
		DesiredPosition += FVector(0.0f, 0.0f, 100.0f);
	}

	// Lerp current value to target value
	auto TargetLean = -10.0f;

	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto HorizontalMovementSpeed = OwningActor->GetVelocity().Size2D();
	TargetLean += FMath::Clamp(HorizontalMovementSpeed * 0.003f, 0.0f, 10.0f);

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

// ============================================================================
// Leg Driver
// ============================================================================

void FDragonTrotLegDriver::Tick(const float DeltaTime)
{
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;

	FProceduralLegDriver::Tick(AdvanceValue);

	// If the leg is stretched too far, disconnect
	if (WalkingState == ELegWalkingState::Planted && Leg->Position.Size() > 350.0f && Leg->Position.Y < 0.0f)
	{
		SetWalkingState(ELegWalkingState::Raised, true);
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

inline constexpr float StepDuration = 0.9f;
inline constexpr float PlantedDuration = 0.45f;
inline constexpr float InertiaDuration = 0.45f;
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
				.Duration = PlantedDuration,
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(0.0f, 400.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 15.0f, 0.0f),
				.LinearForce = 8000.0f,
				.AngularForce = 1.0f,
				.Duration = StepDuration,
				.StartArticulationPosition = FVector(0.0f, 0.0f, 150.0f),
				.StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f),
				.EndArticulationPosition = FVector(0.0f, 0.0f, 0.0f),
				.EndArticulationRotation = FVector(0.0f, 0.0f, 10.0f),
			}
		},
		{ ELegWalkingState::Inertia,
			{
				.TargetPosition = FVector(0.0f, -650.0f, 100.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 70.0f),
				.LinearForce = 3.0f,
				.AngularForce = 0.2f,
				.Duration = InertiaDuration,
			}
		},
	};
	
	return AnimData.at(WalkingState);
}

FDragonTrotPose::FDragonTrotPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDriver = new FDragonTrotBodyDriver(Anim, Anim->ControlledBody.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	HipsDriver = new FDragonWalkHipSwayDriver(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Primary),  Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	BodyDrivers = { BodyDriver };
	HipsDrivers = { HipsDriver };
	
	LeftLegDriver = new FDragonTrotLegDriver(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary));
	RightLegDriver = new FDragonTrotLegDriver(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};

	LeftLegDriver->OnWalkStateChanged.AddLambda([this](ELegWalkingState, const ELegWalkingState NewState)
	{
		if (NewState == ELegWalkingState::Stepping)
			BodyDriver->SetLeadingLeg(LeftLegDriver);
	});
	RightLegDriver->OnWalkStateChanged.AddLambda([this](ELegWalkingState, const ELegWalkingState NewState)
	{
		if (NewState == ELegWalkingState::Stepping)
			BodyDriver->SetLeadingLeg(RightLegDriver);
	});

	// Shares a driver with idle animation
	WingDrivers = {
		new FDragonIdleWingDriver(Anim, Anim->LeftWing.GetBone(EDriverLayer::Primary)),
		new FDragonIdleWingDriver(Anim, Anim->RightWing.GetBone(EDriverLayer::Primary)),
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
	if (LeadingLeg->GetCyclePosition() < PlantedDuration)
	{
		OtherLeg->LockToWorldGround();
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition());
	}
	else
	{
		OtherLeg->SetWalkingState(ELegWalkingState::Inertia);
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition() - PlantedDuration);
	}
}

void FDragonTrotPose::SyncStateFrom(const FDragonSprintPose* SourcePose) const
{
	// BodyDriver->SyncStateFrom(SourcePose->BodyDriver);
	// HipsDriver->SyncStateFrom(SourcePose->HipsDriver);
	LeftLegDriver->SyncStateFrom(SourcePose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(SourcePose->RightLegDriver);

	const auto LeadingLeg = LeftLegDriver->WalkingState == ELegWalkingState::Stepping ? LeftLegDriver : RightLegDriver;
	const auto OtherLeg = LeadingLeg == LeftLegDriver ? RightLegDriver : LeftLegDriver;
	if (LeadingLeg->GetCyclePosition() < PlantedDuration)
	{
		OtherLeg->LockToWorldGround();
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition());
	}
	else
	{
		OtherLeg->SetWalkingState(ELegWalkingState::Inertia);
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition() - PlantedDuration);
	}
}

void FDragonTrotPose::ResetState()
{
	BodyDriver->ResetState();
	HipsDriver->ResetState();
	LeftLegDriver->LockToWorldGround();
	RightLegDriver->SetWalkingState(ELegWalkingState::Stepping);
}

void FDragonTrotPose::Tick(const float DeltaTime)
{
	FProceduralPose::Tick(DeltaTime);
}
