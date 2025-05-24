#include "DragonSprintPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Poses/DragonWalk/DragonWalkPose.h"

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
		SetWalkingState(ELegWalkingState::Inertia, true);
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
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Stepping:
		LockToWorldGround();
		break;
	}
}

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
				.Duration = 1.2f
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(-20.0f, 400.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 15.0f, 0.0f),
				.LinearForce = 80.0f,
				.AngularForce = 1.0f,
				.Duration = 1.2f,
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
				.Duration = 0.5f
			}
		},
	};
	
	return AnimData.at(WalkingState);
}

FDragonSprintPose::FDragonSprintPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	// BodyDriver = new FDragonWalkBodyDriver(Anim, Anim->ControlledBody.GetBone(EDriverLayer::Primary), Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	// HipsDriver = new FDragonWalkHipSwayDriver(Anim, Anim->ControlledHips.GetBone(EDriverLayer::Primary),  Anim->BackLeftLeg.GetBone(EDriverLayer::Primary), Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	// BodyDrivers = { BodyDriver };
	// HipsDrivers = { HipsDriver };
	
	LeftLegDriver = new FDragonSprintLegDriver(Anim, Anim->BackLeftLeg.GetBone(EDriverLayer::Primary));
	RightLegDriver = new FDragonSprintLegDriver(Anim, Anim->BackRightLeg.GetBone(EDriverLayer::Primary));
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
}

void FDragonSprintPose::SyncStateFrom(const FDragonWalkPose* TargetPose) const
{
	// BodyDriver->SyncStateFrom(TargetPose->BodyDriver);
	// HipsDriver->SyncStateFrom(TargetPose->HipsDriver);
	LeftLegDriver->SyncStateFrom(TargetPose->LeftLegDriver);
	RightLegDriver->SyncStateFrom(TargetPose->RightLegDriver);
}

void FDragonSprintPose::ResetState()
{
	// BodyDriver->ResetState();
	// HipsDriver->ResetState();
	LeftLegDriver->LockToWorldGround();
	RightLegDriver->SetWalkingState(ELegWalkingState::Stepping);
}
