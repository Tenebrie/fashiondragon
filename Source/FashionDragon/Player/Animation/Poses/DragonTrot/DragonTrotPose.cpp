#include "DragonTrotPose.h"

#include <map>

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Components/WalkCyclePoseComponent.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverGroundRootSway.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverGroundHipSway.h"
#include "FashionDragon/Player/Animation/Drivers/DragonDriverTurnToMovement.h"
#include "FashionDragon/Player/Animation/Poses/DragonIdle/Drivers/DragonIdleWingDriver.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"
#include "FashionDragon/Player/Animation/Poses/DragonSprint/DragonSprintPose.h"
#include "FashionDragon/Utils/Utils.h"

constexpr float GTrotStepDuration = 0.8f;
constexpr float GTrotPlantedDuration = GTrotStepDuration * 0.6f;
constexpr float GTrotInertiaDuration = GTrotStepDuration - GTrotPlantedDuration;

// ============================================================================
// Pose
// ============================================================================

FDragonTrotPose::FDragonTrotPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	RootDriver = new FDragonDriverGroundRootSway(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));

	const auto RootTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledRoot.GetBone(EDriverLayer::RotateToMovement));
	RootTurnDriver->SetAxisMask(0.0f, 0.5f, 0.0f);
	
	const auto NeckTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledHead.GetBone(EDriverLayer::RotateToMovement));
	NeckTurnDriver->SetAxisMask(FVector(0, 1.5, -0.75));

	const auto BodyTurnDriver = new FDragonDriverTurnToMovement(Anim, Anim->ControlledBody.GetBone(EDriverLayer::RotateToMovement));
	BodyTurnDriver->SetAxisMask(FVector(0, 2.0, -0.75));

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
		if (NewState == ELegWalkingState::Stepping || NewState == ELegWalkingState::FirstStepping)
			RootDriver->SetLeadingLeg(LeftLegDriver);
	});
	RightLegDriver->OnWalkStateChanged.AddLambda([this](ELegWalkingState, const ELegWalkingState NewState)
	{
		if (NewState == ELegWalkingState::Stepping || NewState == ELegWalkingState::FirstStepping)
			RootDriver->SetLeadingLeg(RightLegDriver);
	});

	// Shares a driver with idle animation
	WingDrivers = {
		new FDragonIdleWingDriver(Anim, Anim->LeftWing.GetBone(EDriverLayer::Primary)),
		new FDragonIdleWingDriver(Anim, Anim->RightWing.GetBone(EDriverLayer::Primary)),
	};

	WalkCycleComponent = new FWalkCyclePoseComponent(this, LeftLegDriver, RightLegDriver);
	WalkCycleComponent->SetCycleBreakpoints({
		FBreakpoint(GTrotStepDuration, ELegWalkingState::Stepping),
		FBreakpoint(GTrotPlantedDuration, ELegWalkingState::Planted),
		FBreakpoint(GTrotInertiaDuration, ELegWalkingState::Inertia),
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

	const auto IsLeftStepping = LeftLegDriver->WalkingState == ELegWalkingState::Stepping || LeftLegDriver->WalkingState == ELegWalkingState::FirstStepping;
	const auto LeadingLeg = IsLeftStepping ? LeftLegDriver : RightLegDriver;
	const auto OtherLeg = LeadingLeg == LeftLegDriver ? RightLegDriver : LeftLegDriver;
	if (LeadingLeg->GetCyclePosition() < GTrotPlantedDuration)
	{
		OtherLeg->LockToWorldGround();
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition());
	}
	else
	{
		OtherLeg->SetWalkingState(ELegWalkingState::Inertia);
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition() - GTrotInertiaDuration);
	}

	WalkCycleComponent->SyncStateFrom(SourcePose->WalkCycleComponent);
}

void FDragonTrotPose::SyncStateFrom(const FDragonSprintPose* SourcePose) const
{
	// BodyDriver->SyncStateFrom(SourcePose->BodyDriver);
	// HipsDriver->SyncStateFrom(SourcePose->HipsDriver);
	LeftLegDriver->SyncStateFrom(SourcePose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(SourcePose->RightLegDriver);

	const auto IsLeftStepping = LeftLegDriver->WalkingState == ELegWalkingState::Stepping || LeftLegDriver->WalkingState == ELegWalkingState::FirstStepping;
	const auto LeadingLeg = IsLeftStepping ? LeftLegDriver : RightLegDriver;
	const auto OtherLeg = LeadingLeg == LeftLegDriver ? RightLegDriver : LeftLegDriver;
	if (LeadingLeg->GetCyclePosition() < GTrotPlantedDuration)
	{
		OtherLeg->LockToWorldGround();
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition());
	}
	else
	{
		OtherLeg->SetWalkingState(ELegWalkingState::Inertia);
		OtherLeg->SetCyclePosition(LeadingLeg->GetCyclePosition() - GTrotPlantedDuration);
	}

	WalkCycleComponent->SyncStateFrom(SourcePose->WalkCycleComponent);
}

void FDragonTrotPose::ResetState()
{
	RootDriver->ResetState();
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
	if (BlendAlpha <= 0.0f)
	{
		FBaseDriver::Tick(DeltaTime);
		return;
	}
	
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = MovementSpeed * 0.002f * DeltaTime;
	FProceduralLegSteppingDriver::Tick(AdvanceValue);

	// If the leg is stretched too far, disconnect
	const auto LegReference = RotateVectorToInputRotation(Leg->Position, true);
	const auto ShouldDisconnect = FMath::Abs(LegReference.Y) > 200.f || LegReference.Z < -350.0f || LegReference.X > 600.0f || LegReference.X < -400.0f
		|| FUtils::GetRotatorDistance(Leg->Rotation) > 50.0f;
	if (WalkingState == ELegWalkingState::Planted && ShouldDisconnect)
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
	case ELegWalkingState::FirstStepping:
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
				.TargetRotation = FRotator(-60.0f, 0.0f, 0.0f),
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
				.Duration = GTrotPlantedDuration,
			}
		},
		{ ELegWalkingState::FirstStepping,
			{
				.TargetPosition = FVector(200.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 8000.0f,
				.AngularForce = 1.0f,
				.Duration = GTrotStepDuration / 1.15f,
				.StartArticulationPosition = FVector(0.0f, 0.0f, 50.0f),
				.StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f),
				.EndArticulationPosition = FVector(0.0f, 0.0f, 10.0f),
				.EndArticulationRotation = FVector(0.0f, 0.0f, 10.0f),
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(280.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 8000.0f,
				.AngularForce = 1.0f,
				.Duration = GTrotStepDuration,
				.StartArticulationPosition = FVector(0.0f, 0.0f, 100.0f),
				.StartArticulationRotation = FVector(0.0f, 0.0f, 0.0f),
				.EndArticulationPosition = FVector(0.0f, 0.0f, 10.0f),
				.EndArticulationRotation = FVector(0.0f, 0.0f, 10.0f),
			}
		},
		// TODO: Fix this pls
		{ ELegWalkingState::Inertia,
			{
				.TargetPosition = FVector(-600.0f, 0.0f, 100.0f),
				.TargetRotation = FRotator(-70.0f, 0.0f, 0.0f),
				.LinearForce = 3.0f,
				.AngularForce = 0.2f,
				.Duration = GTrotInertiaDuration,
				.StartArticulationPosition = FVector(-600.0f, 0.0f, 50.0f),
			}
		},
	};
	
	return AnimData.at(WalkingState);
}
