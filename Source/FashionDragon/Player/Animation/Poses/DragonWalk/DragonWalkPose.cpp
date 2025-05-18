#include "DragonWalkPose.h"

#include <map>

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Utils/Utils.h"
#include "GameFramework/CharacterMovementComponent.h"

// ============================================================================
// Body Driver
// ============================================================================

void FDragonWalkBodyDriver::Tick(float DeltaTime)
{
	const auto LeftLegOffset = std::min(1.0, LeftLeg->Position.Size() / 750.0f);
	const auto RightLegOffset = std::min(1.0, RightLeg->Position.Size() / 750.0f);

	const auto LegState = std::min(LeftLegOffset, RightLegOffset);
	const auto VerticalOffset = LegState * -50.0f;

	DesiredPosition = FVector(0.0f, 0.0f, VerticalOffset);

	// Lerp current value to target value
	auto TargetLean = 10.0f + (LegState - 0.5f) * 5.f;

	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (OwningActor->IsSprinting)
	{
		TargetLean += 10.0f;
	}

	const auto Lean = FMath::Lerp(Bone->Rotation.Roll, TargetLean, DeltaTime);
	
	// Rotation = FRotator(0.0f, 0.0f, Lean);
	DesiredRotation = FRotator(0.0f, 0.0f, 0.0f);
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
	const auto LeftLegOffset = std::min(1.0, LeftLeg->Position.Size() / 750.0f);
	const auto RightLegOffset = std::min(1.0, RightLeg->Position.Size() / 750.0f);

	const auto Sway = FMath::Sin((-CyclePosition + 0.5) * PI) * 10.0f;
	
	DesiredRotation = FRotator(0.0f, Sway, 0.0f);
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
		SetWalkingState(ELegWalkingState::Stepping);
		break;
	case ELegWalkingState::Stepping:
		LockWorldGroundPosition();
		break;
	default:
		break;
	}
}

FDragonWalkStateData FDragonWalkLegDriver::GetTargetPosition() const
{
	const std::map<ELegWalkingState, FDragonWalkStateData> AnimData =
	{
		{ ELegWalkingState::Relaxed,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 2500.f,
				.AngularForce = 360.0f,
				.Duration = 1.0f
			}
		},
		{ ELegWalkingState::Raised,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 150.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.LinearForce = 2500.f,
				.AngularForce = 720.0f,
				.Duration = 1.0f
			}
		},
		{ ELegWalkingState::SeekingGround,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 500.f,
				.AngularForce = 10.0f,
				.Duration = 0.2f
			}
		},
		{ ELegWalkingState::Planted,
			{
				.TargetPosition = FVector(0.0f, 0.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 10000.f,
				.AngularForce = 360.0f,
				.Duration = 0.7f
			}
		},
		{ ELegWalkingState::Stepping,
			{
				.TargetPosition = FVector(0.0f, 300.0f, 0.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 0.0f),
				.LinearForce = 25000.f,
				.AngularForce = 360.0f,
				.Duration = 0.7f,
				.StartArticulationPosition = FVector(0.0f, 0.0f, 150.0f),
				.StartArticulationRotation = FVector(0.0f, 0.0f, 60.0f),
				.EndArticulationPosition = FVector(0.0f, 0.0f, 30.0f),
				.EndArticulationRotation = FVector(0.0f, 0.0f, 10.0f),
			}
		},
		{ ELegWalkingState::Inertia,
			{
				.TargetPosition = FVector(0.0f, -250.0f, 200.0f),
				.TargetRotation = FRotator(0.0f, 0.0f, 60.0f),
				.LinearForce = 2000.0f,
				.AngularForce = 360.0f,
				.Duration = 0.5f
			}
		},
	};
	
	const auto Data = FDragonWalkStateData(AnimData.at(WalkingState));
	Data.TargetPosition = RotateVectorToInputRotation(Data.TargetPosition);
	const auto InputRotation = FMath::Abs(FMath::Cos(GetInputRotation()));
	const auto OriginalZ = Data.TargetPosition.Z;
	const auto StepScale = ((InputRotation + 2.0f) / 3.0f);
	Data.TargetPosition *= StepScale;
	Data.TargetPosition.Z = OriginalZ;
	Data.PlaybackSpeed = 1.0f / StepScale;
	return Data;
}

float FDragonWalkLegDriver::GetInputRotation() const
{
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (!OwningActor) { return 0; }
	
	auto VelocityVector = OwningActor->GetCharacterMovement()->GetLastUpdateVelocity();
	VelocityVector.Normalize();
	if (VelocityVector.IsNearlyZero()) { return 0; }
	
	// Get normalized input vector in local space
	const auto InputVector = OwningActor->GetActorRotation().UnrotateVector(VelocityVector).GetSafeNormal2D();

	// Calculate the angle between input vector and forward vector directly
	const float AngleRadians = FMath::Atan2(InputVector.Y, InputVector.X);

	return AngleRadians;
	// return FMath::RadiansToDegrees(AngleRadians);
}

FVector FDragonWalkLegDriver::RotateVectorToInputRotation(const FVector& VectorToRotate, const bool Inverted) const
{
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (!OwningActor) { return VectorToRotate; }
	
	auto VelocityVector = OwningActor->GetCharacterMovement()->GetLastUpdateVelocity();
	VelocityVector.Normalize();
	if (VelocityVector.IsNearlyZero()) { return VectorToRotate; }
	
	// Get normalized input vector in local space
	const auto ActorRotation = OwningActor->GetActorRotation();
	const auto InputVector = ActorRotation.UnrotateVector(VelocityVector).GetSafeNormal2D();

	// Calculate the angle between input vector and forward vector directly
	float AngleRadians = FMath::Atan2(InputVector.Y, InputVector.X);
	if (Inverted)
	{
		AngleRadians = -AngleRadians;
	}

	// Create rotation matrix around Z axis using the angle
	const FRotator Rotation(0, FMath::RadiansToDegrees(AngleRadians), 0);
	const FRotationMatrix RotMatrix(Rotation);

	// Apply rotation to target position
	return RotMatrix.TransformPosition(VectorToRotate);
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
	BodyDriver = new FDragonWalkBodyDriver(Anim, Anim->ControlledBody, Anim->BackLeftLeg, Anim->BackRightLeg);
	HipsDriver = new FDragonWalkHipSwayDriver(Anim, Anim->ControlledHips, Anim->BackLeftLeg, Anim->BackRightLeg);
	FProceduralPose::BodyDriver = BodyDriver;
	FProceduralPose::HipsDriver = HipsDriver;

	LeftLegDriver = new FDragonWalkLegDriver(Anim, Anim->BackLeftLeg);
	RightLegDriver = new FDragonWalkLegDriver(Anim, Anim->BackRightLeg);
	LegDrivers = {
		LeftLegDriver,
		RightLegDriver,
	};
}

void FDragonWalkPose::SyncStateFrom(const FDragonTrotPose* TargetPose) const
{
	BodyDriver->SyncStateFrom(TargetPose->BodyDriver);
	HipsDriver->SyncStateFrom(TargetPose->HipsDriver);
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

	PushingLeg->LockWorldGroundPosition();
	SteppingLeg->SetWalkingState(ELegWalkingState::Stepping);

	SwitchStartingLeg = !SwitchStartingLeg;
}
