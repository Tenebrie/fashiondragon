#include "DragonSprintPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Drivers/DragonIdleWingDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonTrot/DragonTrotPose.h"

FDragonSprintLegDriver::FDragonSprintLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegDriver(AnimInstance, ControlledLeg)
{}

// ============================================================================
// Leg Driver
// ============================================================================
void FDragonSprintLegDriver::Tick(const float DeltaTime)
{
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;

	FProceduralLegDriver::Tick(AdvanceValue);

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

inline constexpr float StepDuration = 1.2f;
inline constexpr float PlantedDuration = 0.7f;
inline constexpr float InertiaDuration = 0.5f;
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
				.Duration = PlantedDuration
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(-20.0f, 400.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 15.0f, 0.0f),
				.LinearForce = 80.0f,
				.AngularForce = 1.0f,
				.Duration = StepDuration,
				.StartArticulationPosition = FVector(0.0f, 0.0f, 0.0f),
				.StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f),
				.EndArticulationPosition = FVector(0.0f, 0.0f, 30.0f),
				.EndArticulationRotation = FVector(0.0f, 0.0f, 10.0f),
			}
		},
		{ ELegWalkingState::Inertia,
			{
				.TargetPosition = FVector(0.0f, -450.0f, 180.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.LinearForce = 2.0f,
				.AngularForce = 1.0f,
				.Duration = InertiaDuration
			}
		},
	};
	
	return AnimData.at(WalkingState);
}

FDragonSprintPose::FDragonSprintPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDriver = new FDragonWalkBodyDriver(Anim, Anim->ControlledBody.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	HipsDriver = new FDragonWalkHipSwayDriver(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Primary),  Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	BodyDrivers = { BodyDriver };
	HipsDrivers = { HipsDriver };
	
	LeftLegDriver = new FDragonSprintLegDriver(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary));
	RightLegDriver = new FDragonSprintLegDriver(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
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

void FDragonSprintPose::SyncStateFrom(const FDragonWalkPose* SourcePose) const
{
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

void FDragonSprintPose::SyncStateFrom(const FDragonTrotPose* SourcePose) const
{
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

void FDragonSprintPose::ResetState()
{
	BodyDriver->ResetState();
	HipsDriver->ResetState();
	LeftLegDriver->LockToWorldGround();
	RightLegDriver->SetWalkingState(ELegWalkingState::Stepping);
}
