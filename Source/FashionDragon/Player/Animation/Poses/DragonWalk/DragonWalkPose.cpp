#include "DragonWalkPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverGroundHipSway.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Drivers/DragonIdleWingDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonSprint/DragonSprintPose.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverGroundRootSway.h"
// ReSharper disable once CppUnusedIncludeDirective False positive
#include "FashionDragon/Player/Animation/Components/WalkCyclePoseComponent.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverTurnToMovement.h"
#include "FashionDragon/Player/Animation/Poses/DragonTrot/DragonTrotPose.h"
#include "FashionDragon/Utils/Utils.h"

// ============================================================================
// Leg Driver
// ============================================================================

FDragonWalkLegDriver::FDragonWalkLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FProceduralLegSteppingDriver(AnimInstance, ControlledLeg)
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

#define STEP_DURATION 0.9f
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
				.TargetPosition = FVector(0.0f, 0.0f, 100.0f),
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
				.Duration = STEP_DURATION
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(0, 300.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 2.0f,
				.AngularForce = 1.0f,
				.Duration = STEP_DURATION,
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

	FProceduralLegSteppingDriver::Tick(AdvanceValue);
	
	// If the leg is stretched too far, disconnect
	const auto LegReference = RotateVectorToInputRotation(Leg->Position, true);
	const auto ShouldDisconnect = FMath::Abs(LegReference.X) > 200.f || LegReference.Z < -150.0f || LegReference.Y > 600.0f || LegReference.Y < -300.0f
		|| FUtils::GetRotatorDistance(Leg->Rotation) > 50.0f;
	if (WalkingState == ELegWalkingState::Planted && ShouldDisconnect)
	{
		SetWalkingState(ELegWalkingState::Raised);
	}
}

FDragonWalkPose::FDragonWalkPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	BodyDriver = new FDragonDriverGroundRootSway(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	BodyDriver->SetHorizontalAmplitude(2.0f);
	BodyDriver->SetVerticalAmplitude(200.0f);

	const auto RootTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::RotateToMovement));
	RootTurnDriver->SetAxisMask(0.75f, 0.0f, 0.0f);

	const auto BodyTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledBody.GetBone(EDriverLayer::RotateToMovement));
	BodyTurnDriver->SetAxisMask(FVector(0, 3, 0));

	HipsDriver = new FDragonDriverGroundHipSway(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	
	BoneDrivers = { BodyDriver, RootTurnDriver, BodyTurnDriver, HipsDriver };

	LeftLegDriver = new FDragonWalkLegDriver(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary));
	RightLegDriver = new FDragonWalkLegDriver(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
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

	WalkCycleComponent = new FWalkCyclePoseComponent(this, LeftLegDriver, RightLegDriver);
	WalkCycleComponent->SetCycleBreakpoints({
		FBreakpoint(STEP_DURATION, ELegWalkingState::Stepping),
		FBreakpoint(STEP_DURATION, ELegWalkingState::Planted),
	});
	Components = {
		WalkCycleComponent,
	};
}

template<typename DriverT>
void FDragonWalkPose::SyncStateFrom(const DriverT* SourcePose) const
{
	LeftLegDriver->SyncStateFrom(SourcePose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(SourcePose->RightLegDriver);

	// const auto LeadingLeg = LeftLegDriver->WalkingState == ELegWalkingState::Stepping ? LeftLegDriver : RightLegDriver;
	// const auto OtherLeg = LeadingLeg == LeftLegDriver ? RightLegDriver : LeftLegDriver;
	// if (LeadingLeg->GetCyclePosition() < PlantedDuration)
	// {
	// 	OtherLeg->LockToWorldGround();
	// 	OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition());
	// }
	// else
	// {
	// 	OtherLeg->SetWalkingState(ELegWalkingState::Relaxed);
	// 	OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition() - PlantedDuration);
	// }
	WalkCycleComponent->SyncStateFrom(SourcePose->WalkCycleComponent);
}

template void FDragonWalkPose::SyncStateFrom(const FDragonTrotPose*) const;
template void FDragonWalkPose::SyncStateFrom(const FDragonSprintPose*) const;

// TODO: If it's not been enough time since we have been here, don't reset, but continue the previous state
void FDragonWalkPose::ResetState()
{
	const auto PushingLeg = SwitchStartingLeg ? RightLegDriver : LeftLegDriver;
	const auto SteppingLeg = SwitchStartingLeg ? LeftLegDriver : RightLegDriver;

	PushingLeg->LockToWorldGround();
	SteppingLeg->SetWalkingState(ELegWalkingState::Stepping);

	SwitchStartingLeg = !SwitchStartingLeg;

	WalkCycleComponent->ResetState();
}
