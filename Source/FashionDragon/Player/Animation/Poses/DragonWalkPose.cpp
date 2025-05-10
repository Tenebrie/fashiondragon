#include "DragonWalkPose.h"

#include <map>

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

FDragonWalkLegDriver::FDragonWalkLegDriver(UDragonAnimInstance* AnimInstance, FControlledLeg* ControlledLeg): FAbstractProceduralLegDriver(AnimInstance, ControlledLeg)
{}

// ============================================================================
// Leg Driver
// ============================================================================
void FDragonWalkLegDriver::Tick(const float DeltaTime)
{
	ArticulationPosition = FVector(0.f, 0.f, 0.f);
	// If we're stepping forward, add some vertical offset
	if (WalkingState == ELegWalkingState::Stepping)
		ArticulationPosition.Z = 100.f;
	
	// Advance time forward. Adjusted by character's movement speed.
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;
	
	FAbstractProceduralLegDriver::Tick(AdvanceValue);
}

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
		SetWalkingState(ELegWalkingState::Planted);
		break;
	}
}

std::pair<FVector, FRotator> FDragonWalkLegDriver::GetTargetPosition() const
{
	const std::map<ELegWalkingState, std::pair<FVector, FRotator>> LegStateToPosition =
	{
		{ ELegWalkingState::Relaxed, LEG_POSE((0.0f, 0.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Raised, LEG_POSE((0.0f, 0.0f, 150.0f), (0.0f, 0.0f, 60.0f)) },
		{ ELegWalkingState::Planted, LEG_POSE((0.0f, -350.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Stepping, LEG_POSE((0.0f, 500.0f, 0.0f), (0.0f, 0.0f, 0.0f)) },
		{ ELegWalkingState::Inertia, LEG_POSE((0.0f, -350.0f, 200.0f), (0.0f, 0.0f, 60.0f)) },
	};
	
	return LegStateToPosition.at(WalkingState);
}

FDragonWalkPose::FDragonWalkPose(UDragonAnimInstance* AnimInstance, FControlledLeg* LeftLeg, FControlledLeg* RightLeg): FAbstractProceduralPose(AnimInstance)
{
	const auto LeftDriver = new FDragonWalkLegDriver(AnimInstance, LeftLeg);
	const auto RightDriver = new FDragonWalkLegDriver(AnimInstance, RightLeg);
	LegDrivers = {
		LeftDriver,
		RightDriver,
	};
	LeftDriver->SetWalkingState(Planted);
}
